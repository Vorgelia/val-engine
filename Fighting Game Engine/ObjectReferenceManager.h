#pragma once
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "BaseObject.h"

class ObjectReferenceManager
{
	friend class BaseObject;

protected:
	static std::unique_ptr<ObjectReferenceManager> _instance;

public:
	static ObjectReferenceManager& Get();

protected:
	std::unordered_set<const BaseObject*> _objectLookup;

	void AddObject(const BaseObject* object);
	void RemoveObject(const BaseObject* object);

public:
	bool IsObjectValid(const BaseObject* object);

protected:
	ObjectReferenceManager() = default;
public:
	~ObjectReferenceManager();
};

template<typename ObjectT = BaseObject>
struct ObjectReference
{
protected:
	mutable ObjectT* _referencedObject;

public:
	ObjectT* get() const;
	bool IsValid() const { return get() != nullptr; }

	void Reset(ObjectT* newObject);

	explicit operator ObjectT*() const { return get(); }
	ObjectT* operator->() const { return get(); }
	ObjectT& operator*() const { return *get(); }
	ObjectReference& operator=(ObjectT* rhs);

	bool operator==(const ObjectReference<ObjectT>& other) const { return get() == other.get(); }

	template<typename TargetObjectT>
	operator ObjectReference<TargetObjectT>() const { return ObjectReference<TargetObjectT>(dynamic_cast<TargetObjectT>(get())); }

	explicit ObjectReference(ObjectT* object);
	explicit ObjectReference(std::nullptr_t);
	ObjectReference();
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
	_referencedObject = newObject;
}

template <typename ObjectT>
ObjectReference<ObjectT>& ObjectReference<ObjectT>::operator=(ObjectT* rhs)
{
	Reset(rhs);
	return *this;
}

template <typename ObjectT>
ObjectReference<ObjectT>::ObjectReference(ObjectT* object)
	: _referencedObject(object)
{
}

template <typename ObjectT>
ObjectReference<ObjectT>::ObjectReference(std::nullptr_t)
	: _referencedObject(nullptr)
{
	
}

template <typename ObjectT>
ObjectReference<ObjectT>::ObjectReference()
	: _referencedObject(nullptr)
{
	
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