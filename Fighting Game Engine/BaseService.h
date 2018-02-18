#pragma once
class ServiceManager;

class BaseService
{
protected:
	ServiceManager* _serviceManager;

	bool _allowServiceUpdate;
	const int _serviceUpdateSortingOrder;

public:
	bool allowServiceUpdate() const;
	int serviceUpdateSortingOrder() const;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Cleanup() = 0;

	BaseService& operator=(BaseService&) = delete;
	BaseService& operator=(BaseService&&) = delete;

	BaseService(BaseService& service) = delete;
	BaseService(BaseService&& service) = delete;

	BaseService(ServiceManager* serviceManager, int sortingOrder = 0);
	~BaseService();
};

