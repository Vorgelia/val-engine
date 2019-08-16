#pragma once
#include "IReflectable.h"
#include "ObjectFactory.h"
#include <unordered_set>
#include "ObjectReferenceManager.h"

class GameScene;
class GameInstance;

#define VE_OBJECT_DECLARATION(ObjectT, SuperClass) \
	private:\
	typedef SuperClass Super;\
	public:\
	std::string className() const override { return #ObjectT; }\
	static BaseObject* StaticClass();\
	BaseObject* GetClass() const override;\
	private:

#define VE_OBJECT_DEFINITION(ObjectT)\
	bool _registered##ObjectT = (ObjectT::StaticClass() != nullptr) && ObjectFactory::RegisterObjectGenerator<ObjectT>(#ObjectT);\
	BaseObject* ObjectT::StaticClass()\
	{\
		static ve::unique_object_ptr<ObjectT> _staticClass { new ObjectT() };\
		return _staticClass.get();\
	}\
	BaseObject* ObjectT::GetClass() const\
	{\
		return ObjectT::StaticClass();\
	}\

#define VE_REGISTER_CUSTOM_UPDATE_FUNCTION(UpdateDispatcher, UpdateGroup, UpdateType, UpdateFunction)\
	UpdateDispatcher.BindFunction(this, UpdateFunctionTiming((UpdateGroup), (UpdateType)), [this](){UpdateFunction();})

#define VE_REGISTER_UPDATE_FUNCTION(UpdateGroup, UpdateType, UpdateFunction)\
	VE_REGISTER_CUSTOM_UPDATE_FUNCTION(_owningInstance->updateDispatcher(), UpdateGroup, UpdateType, UpdateFunction)

class BaseObject : public IReflectable
{
	friend class ObjectInitializer;
	friend class ObjectReferenceManager;
	friend struct ve::ObjectDeleter;

private:
	std::unordered_set<const ObjectReference<BaseObject>*> _references;

	void RegisterReference(const ObjectReference<BaseObject>& reference);
	void UnregisterReference(const ObjectReference<BaseObject>& reference);
	void InvalidateReferences();
	
protected:
	GameInstance* _owningInstance;
	BaseObject* _outer;

public:
	bool enabled;

	static BaseObject* StaticClass();
	virtual BaseObject* GetClass() const;
	virtual std::string className() const { return "BaseObject"; }

	GameInstance* owningInstance() const { return _owningInstance; }

	virtual void OnSerialized(BaseSerializationProxy& proxy) const override;

	virtual void OnInit() {}
	virtual void OnDestroyed() {}

	BaseObject();
	virtual ~BaseObject();
};