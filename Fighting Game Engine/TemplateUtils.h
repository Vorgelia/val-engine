#pragma once
#include "ValEngine.h"
#include "ObjectPtr.h"

namespace ValEngine
{
	template<typename Test, template<typename...> class Ref>
	struct is_specialization : std::false_type {};
	template<template<typename...> class Ref, typename... Args>
	struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

	template<typename T>
	struct is_object_ptr : std::false_type {};
	template<typename T>
	struct is_object_ptr<ve::unique_object_ptr<T>> : std::true_type {};
	template<typename T>
	static constexpr bool is_object_ptr_v = is_object_ptr<T>::value;

	template<typename T>
	struct remove_object_ptr { typedef T type; };
	template<typename T>
	struct remove_object_ptr<ve::unique_object_ptr<T>> { typedef T type; };
	template<typename T>
	using remove_object_ptr_t = typename remove_object_ptr<T>::type;
}