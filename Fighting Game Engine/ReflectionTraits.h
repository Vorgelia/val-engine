#pragma once
#include "ValEngine.h"
class IReflectable;

enum class ReflectionFieldType
{
	Unknown = 0,
	Int = 1,
	Float = 2,
	Dec = 3,
	Bool = 4,
	String = 5,
	Reflectable = 6,
	Array = 7,
	Map = 8
};

template<typename VariableT>
struct ReflectionFieldTraits
{
	static constexpr const ReflectionFieldType field_type = ReflectionFieldType::Unknown;
};

template<ReflectionFieldType TypeE>
struct ReflectionTypeTraits
{
	typedef std::nullptr_t value_type;
	static constexpr bool is_serializable = false;
};

#define DECLARE_FIELD_TRAITS(varType, fieldType)\
	template<>\
	struct ReflectionFieldTraits<varType> { static constexpr ReflectionFieldType field_type = fieldType; }

#define DECLARE_TYPE_TRAITS(varType)\
	template<>\
	struct ReflectionTypeTraits<ReflectionFieldTraits<varType>::field_type> { typedef varType value_type; static constexpr bool is_serializable = true; }

#define DECLARE_REFLECTION_TRAITS(varType, valueType)\
	DECLARE_FIELD_TRAITS(varType, valueType);\
	DECLARE_TYPE_TRAITS(varType);

DECLARE_REFLECTION_TRAITS(int, ReflectionFieldType::Int);
DECLARE_REFLECTION_TRAITS(bool, ReflectionFieldType::Bool);
DECLARE_REFLECTION_TRAITS(float, ReflectionFieldType::Float);
DECLARE_REFLECTION_TRAITS(ve::dec_t, ReflectionFieldType::Dec);
DECLARE_REFLECTION_TRAITS(std::string, ReflectionFieldType::String);
DECLARE_REFLECTION_TRAITS(IReflectable, ReflectionFieldType::Reflectable);