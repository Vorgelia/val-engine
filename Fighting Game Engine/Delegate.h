#pragma once
#include <vector>
#include <functional>
#include "ContainerUtils.h"

#define VE_DELEGATE_FUNC(type, name) \
	type::registered_func_t(this, type::func_t{[this](auto ...args){ name(args...); }})

class BaseObject;

template<typename... argTypes>
class Delegate
{
public:
	typedef std::function<void(argTypes...)> func_t;
	typedef std::pair<ObjectReference<BaseObject>, func_t> registered_func_t;

protected:
	std::vector<registered_func_t> _listeners;

public:
	void operator()(argTypes... types);
	void operator+=(registered_func_t& listener);
	void operator-=(registered_func_t& listener);
	void operator=(Delegate<argTypes...>& listener) = delete;
	void operator=(Delegate<argTypes...>&& listener) = delete;

	void Invoke(argTypes... types) { this->operator()(types...); }
	void Clear();

	Delegate() = default;
	Delegate(Delegate<argTypes...>&) = delete;
	Delegate(Delegate<argTypes...>&&) = delete;
	~Delegate() = default;
};

template<typename ...argTypes>
inline void Delegate<argTypes...>::operator()(argTypes ...types)
{
	for(int i = 0; i < _listeners.size() ;)
	{
		if(_listeners[i].first.IsValid())
		{
			_listeners[i].second(types...);
			++i;
		}
		else
		{
			ve::erase_swap(_listeners, i);
		}
	}
}

template<typename ...argTypes>
inline void Delegate<argTypes...>::operator+=(registered_func_t& listener)
{
	_listeners.push_back(listener);
}

template<typename ...argTypes>
inline void Delegate<argTypes...>::operator-=(registered_func_t& listener)
{
	for(auto& iter = _listeners.begin(); iter != _listeners.end(); ++iter)
	{
		if((iter->first == listener.first) && iter->second.target<func_t>() == listener.second.target<func_t>())
		{
			_listeners.erase(iter);
			break;
		}
	}
}

template<typename ...argTypes>
inline void Delegate<argTypes...>::Clear()
{
	_listeners.clear();
}
