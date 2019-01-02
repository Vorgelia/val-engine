#pragma once
#include "IReflectable.h"
#include "ObjectFactory.h"
#include <unordered_set>
#include "ObjectReferenceManager.h"

class GameScene;
class GameInstance;

#define VE_OBJECT_DECLARATION(ObjectT) \
	public:\
	virtual std::string className() const override { return #ObjectT; }\
	static BaseObject* StaticClass();\
	virtual BaseObject* GetClass() const override;\
	private:

#define VE_OBJECT_DEFINITION(ObjectT)\
	bool _registered##ObjectT = (ObjectT::StaticClass() != nullptr) && ObjectFactory::RegisterObjectGenerator<ObjectT>(#ObjectT);\
	BaseObject* ObjectT::StaticClass()\
	{\
		static std::unique_ptr<ObjectT> _staticClass = std::make_unique<ObjectT>();\
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

private:
	std::unordered_set<const ObjectReference<BaseObject>*> _references;

	void RegisterReference(const ObjectReference<BaseObject>& reference);
	void UnregisterReference(const ObjectReference<BaseObject>& reference);

protected:
	GameInstance* _owningInstance;
	BaseObject* _outer;

public:
	bool enabled;

	static BaseObject* StaticClass();
	virtual BaseObject* GetClass() const;
	virtual std::string className() const { return "BaseObject"; }

	GameInstance* owningInstance() const { return _owningInstance; }

	virtual json Serialize() const override;

	virtual void OnInit() {}
	virtual void OnDestroyed() {}

	BaseObject();
	virtual ~BaseObject();
};