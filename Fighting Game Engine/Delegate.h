#pragma once
#include <vector>
#include <functional>

#define VE_DELEGATE_FUNC(type, name) \
	type::func_t([this](auto ...args){ name(args...); })

template<typename... argTypes>
class Delegate
{
public:
	typedef std::function<void(argTypes...)> func_t;
protected:
	std::vector<func_t> _listeners;

public:
	void operator()(argTypes... types);
	void operator+=(func_t& listener);
	void operator-=(func_t& listener);
	void operator=(Delegate<argTypes...>& listener) = delete;
	void operator=(Delegate<argTypes...>&& listener) = delete;

	void Clear();

	Delegate() = default;
	Delegate(Delegate<argTypes...>&) = delete;
	Delegate(Delegate<argTypes...>&&) = delete;
	~Delegate() = default;
};

template<typename ...argTypes>
inline void Delegate<argTypes...>::operator()(argTypes ...types)
{
	for(auto& iter : _listeners)
	{
		iter(types...);
	}
}

template<typename ...argTypes>
inline void Delegate<argTypes...>::operator+=(func_t& listener)
{
	_listeners.push_back(listener);
}

template<typename ...argTypes>
inline void Delegate<argTypes...>::operator-=(func_t& listener)
{
	for(auto& iter = _listeners.begin(); iter != _listeners.end(); ++iter)
	{
		if(iter->template target<void(argTypes...)>() == listener.template target<void(argTypes...)>())
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
