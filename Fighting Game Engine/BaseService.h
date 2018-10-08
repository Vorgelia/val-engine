#pragma once
#include "BaseObject.h"
class GameInstance;

class BaseService : public BaseObject
{
public:
	BaseService& operator=(BaseService&) = delete;
	BaseService& operator=(BaseService&&) = delete;

	BaseService(BaseService& service) = delete;
	BaseService(BaseService&& service) = delete;

	BaseService();
	virtual ~BaseService() = default;
};

