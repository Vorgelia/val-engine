#pragma once
#include "BaseObject.h"
class GameInstance;

class BaseService : public BaseObject
{
	friend class GameInstance;

	VE_OBJECT_DECLARATION(BaseService, BaseObject);

protected:
	virtual void OnServiceInit() {}
	virtual void OnServiceCleanup() {}

public:
	BaseService& operator=(BaseService&) = delete;
	BaseService& operator=(BaseService&&) = delete;

	BaseService(BaseService& service) = delete;
	BaseService(BaseService&& service) = delete;

	BaseService()  = default;
	virtual ~BaseService() = default;
};
