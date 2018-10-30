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
	~ObjectReferenceManager() = default;
};

template<typename ObjectT = BaseObject>
struct ObjectReference
{
	static_assert(std::is_same_v<BaseObject, ObjectT> || std::is_base_of_v<BaseObject, ObjectT>, "ObjectReference type is not derived from BaseObject");

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

	ObjectReference(ObjectT* object);
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
	if(!ObjectReferenceManager::Get().IsObjectValid(_referencedObject))
	{
		_referencedObject = nullptr;
		return;
	}
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
