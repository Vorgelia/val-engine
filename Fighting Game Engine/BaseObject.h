#pragma once
#include "IReflectable.h"
#include "ObjectFactory.h"

class GameScene;
class GameInstance;

#define VE_OBJECT_DECLARATION(ObjectT) \
	public:\
	virtual std::string className() const override { return #ObjectT; }\
	static BaseObject* StaticClass();\
	private:

#define VE_OBJECT_DEFINITION(GenType)\
	bool _registered##GenType = (GenType::StaticClass() != nullptr) && ObjectFactory::RegisterObjectGenerator<GenType>(#GenType);\
	BaseObject* GenType::StaticClass()\
	{\
		static std::unique_ptr<GenType> _staticClass = std::make_unique<GenType>();\
		return _staticClass.get();\
	}

class BaseObject : public IReflectable
{
	friend class ObjectInitializer;

protected:
	GameInstance* _owningInstance;
	BaseObject* _outer;

	virtual json Serialize() const override;

public:
	static BaseObject* StaticClass();
	virtual std::string className() const { return "BaseObject"; }

	GameInstance* owningInstance() const { return _owningInstance; }

	virtual void OnInit() {}
	virtual void OnDestroyed() {}

	BaseObject();
	virtual ~BaseObject();
};