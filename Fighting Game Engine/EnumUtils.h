#pragma once
#include <type_traits>

#define VE_DECLARE_BITMASK_ENUM(EnumType) \
	namespace ValEngine\
	{\
		template<>\
		struct is_bitmask<EnumType> : std::bool_constant<std::is_enum_v<EnumType>> {}; \
	}

#define VE_BITMASK_VALUE_NONE None = 0b0
#define VE_BITMASK_VALUE_ANY Any = 0b11111111
#define VE_BITMASK_VALUE_ALL All = 0b11111111

namespace ValEngine
{
	template<typename EnumT, typename std::enable_if_t<std::is_enum_v<EnumT>, EnumT> = nullptr>
	struct is_bitmask : std::bool_constant<false> {};

	template<typename EnumT>
	bool is_bitmask_v = is_bitmask<EnumT>::value;
}

#define VE_STATIC_ASSERT_IS_BITMASK(type) static_assert(ValEngine::is_bitmask_v<(type)>, "template parameter is not a bitmask")

template<typename EnumT>
EnumT operator |(EnumT lhs, EnumT rhs)
{
	VE_STATIC_ASSERT_IS_BITMASK(EnumT);

	using underlying = typename std::underlying_type<EnumT>::type;

	return static_cast<EnumT> (
		static_cast<underlying>(lhs) |
		static_cast<underlying>(rhs)
		);
}
template<typename EnumT>
EnumT& operator |=(EnumT& lhs, EnumT rhs)
{
	VE_STATIC_ASSERT_IS_BITMASK(EnumT);

	using underlying = typename std::underlying_type<EnumT>::type;

	lhs = static_cast<EnumT> (
		static_cast<underlying>(lhs) |
		static_cast<underlying>(rhs)
		);

	return lhs;
}
template<typename EnumT>
EnumT operator &(EnumT lhs, EnumT rhs)
{
	VE_STATIC_ASSERT_IS_BITMASK(EnumT);

	using underlying = typename std::underlying_type<EnumT>::type;

	return static_cast<EnumT> (
		static_cast<underlying>(lhs) &
		static_cast<underlying>(rhs)
		);
}
template<typename EnumT>
EnumT& operator &=(EnumT& lhs, EnumT rhs)
{
	VE_STATIC_ASSERT_IS_BITMASK(EnumT);

	using underlying = typename std::underlying_type<EnumT>::type;

	lhs = static_cast<EnumT> (
		static_cast<underlying>(lhs) &
		static_cast<underlying>(rhs)
		);

	return lhs;
}
template<typename EnumT>
EnumT operator ^(EnumT lhs, EnumT rhs)
{
	VE_STATIC_ASSERT_IS_BITMASK(EnumT);

	using underlying = typename std::underlying_type<EnumT>::type;

	return static_cast<EnumT> (
		static_cast<underlying>(lhs) ^
		static_cast<underlying>(rhs)
		);
}
template<typename EnumT>
EnumT& operator ^=(EnumT& lhs, EnumT rhs)
{
	VE_STATIC_ASSERT_IS_BITMASK(EnumT);

	using underlying = typename std::underlying_type<EnumT>::type;

	lhs = static_cast<EnumT> (
		static_cast<underlying>(lhs) ^
		static_cast<underlying>(rhs)
		);

	return lhs;
}

template<typename EnumT>
EnumT operator ~(EnumT lhs)
{
	VE_STATIC_ASSERT_IS_BITMASK(EnumT);

	using underlying = typename std::underlying_type<EnumT>::type;

	return static_cast<EnumT> (
		~static_cast<underlying>(lhs)
		);
}