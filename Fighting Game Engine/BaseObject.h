#pragma once
#include "IReflectable.h"
#include "ObjectFactory.h"

class GameScene;
class GameInstance;

#define VE_OBJECT_DECLARATION(ObjectT) \
	public:\
	virtual std::string className() const override { return #ObjectT; }\
	static BaseObject* StaticClass();\
	virtual BaseObject* GetClass() const override;\
	private:

#define VE_OBJECT_DEFINITION(GenType)\
	bool _registered##GenType = (GenType::StaticClass() != nullptr) && ObjectFactory::RegisterObjectGenerator<GenType>(#GenType);\
	BaseObject* GenType::StaticClass()\
	{\
		static std::unique_ptr<GenType> _staticClass = std::make_unique<GenType>();\
		return _staticClass.get();\
	}\
	BaseObject* GenType::GetClass() const\
	{\
		return GenType::StaticClass();\
	}\

#define VE_REGISTER_CUSTOM_UPDATE_FUNCTION(UpdateDispatcher, UpdateGroup, UpdateType, UpdateFunction)\
	UpdateDispatcher.BindFunction(this, UpdateFunctionTiming((UpdateGroup), (UpdateType)), [this](){UpdateFunction();})

#define VE_REGISTER_UPDATE_FUNCTION(UpdateGroup, UpdateType, UpdateFunction)\
	VE_REGISTER_CUSTOM_UPDATE_FUNCTION(_owningInstance->updateDispatcher(), UpdateGroup, UpdateType, UpdateFunction)


class BaseObject : public IReflectable
{
	friend class ObjectInitializer;

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