#pragma once
#include <memory>
#include <unordered_map>
#include <unordered_set>

class ObjectReferenceManager
{
	friend class BaseObject;
	template<typename ObjectT>
	friend struct ObjectReference;

protected:
	static std::unique_ptr<ObjectReferenceManager> _instance;

public:
	static ObjectReferenceManager& Get();
	static ObjectReferenceManager* TryGet();

protected:
	std::unordered_set<const BaseObject*> _objectLookup;

	void AddObject(const BaseObject* object);
	void RemoveObject(const BaseObject* object);

	void RegisterReferenceToObject(BaseObject* object, const ObjectReference<BaseObject>& reference);
	void UnregisterReferenceToObject(BaseObject* object, const ObjectReference<BaseObject>& reference);

public:
	bool IsObjectValid(const BaseObject* object);

protected:
	ObjectReferenceManager() = default;
public:
	~ObjectReferenceManager();
};

class BaseObject;

template<typename ObjectT = BaseObject>
struct ObjectReference
{
	friend class BaseObject;

protected:
	mutable ObjectT* _referencedObject;

	void Invalidate() const { _referencedObject = nullptr; }

public:
	ObjectT* get() const;
	bool IsValid() const { return get() != nullptr; }

	void Reset(ObjectT* newObject);

	explicit operator ObjectT*() const { return get(); }
	ObjectT* operator->() const { return get(); }
	ObjectT& operator*() const { return *get(); }
	ObjectReference& operator=(ObjectT* rhs);
	ObjectReference& operator=(const ObjectReference<ObjectT>& rhs);
	ObjectReference& operator=(ObjectReference<ObjectT>&& rhs);

	template<typename TargetObjectT>
	operator ObjectReference<TargetObjectT>() const { return ObjectReference<TargetObjectT>(static_cast<TargetObjectT*>(get())); }

	bool operator==(const ObjectReference<ObjectT>& other) const { return get() == other.get(); }

	explicit ObjectReference(ObjectT* object);
	ObjectReference(const ObjectReference<ObjectT>& object);
	ObjectReference(ObjectReference<ObjectT>&& object);
	ObjectReference();
	~ObjectReference();
};

template <typename ObjectT>
ObjectT* ObjectReference<ObjectT>::get() const
{
	if(!ObjectReferenceManager::Get().IsObjectValid(_referencedObject))
	{
		_referencedObject = nullptr;
	}

	return _referencedObject;
}

template <typename ObjectT>
void ObjectReference<ObjectT>::Reset(ObjectT* newObject)
{
	ObjectReferenceManager::Get().UnregisterReferenceToObject(_referencedObject, *this);

	_referencedObject = newObject;

	ObjectReferenceManager::Get().RegisterReferenceToObject(_referencedObject, *this);
}

template <typename ObjectT>
ObjectReference<ObjectT>& ObjectReference<ObjectT>::operator=(ObjectT* rhs)
{
	Reset(rhs);
	return *this;
}

template <typename ObjectT>
ObjectReference<ObjectT>& ObjectReference<ObjectT>::operator=(const ObjectReference<ObjectT>& rhs)
{
	Reset(rhs.get());
	return *this;
}

template <typename ObjectT>
ObjectReference<ObjectT>& ObjectReference<ObjectT>::operator=(ObjectReference<ObjectT>&& rhs)
{
	Reset(rhs.get());
	return *this;
}

template <typename ObjectT>
ObjectReference<ObjectT>::ObjectReference(ObjectT* object)
{
	Reset(object);
}

template <typename ObjectT>
ObjectReference<ObjectT>::ObjectReference(const ObjectReference<ObjectT>& object)
{
	Reset(object.get());
}

template <typename ObjectT>
ObjectReference<ObjectT>::ObjectReference(ObjectReference<ObjectT>&& object)
{
	Reset(object.get());
}

template <typename ObjectT>
ObjectReference<ObjectT>::ObjectReference()
	: _referencedObject(nullptr)
{

}

template <typename ObjectT>
ObjectReference<ObjectT>::~ObjectReference()
{
	if(_referencedObject != nullptr && ObjectReferenceManager::TryGet() != nullptr)
		ObjectReferenceManager::Get().UnregisterReferenceToObject(_referencedObject, *this);
}

namespace std
{
	template <typename ObjectT>
	struct hash<ObjectReference<ObjectT>>
	{
		size_t operator()(const ObjectReference<ObjectT> & x) const
		{
			return std::hash<ObjectT*>().operator()(x.get());
		}
	};
}
