#pragma once
#include "EnumUtils.h"

//TODO: Generalize
enum class InputButton : uint16_t
{
	VE_BITMASK_VALUE_NONE,
	Light	= 0b0000001,
	Medium	= 0b0000010,
	Heavy	= 0b0000100,
	Attacks = 0b0000111,//Convenience entry that represents any of the attacks
	Shield	= 0b0001000,
	Buttons	= 0b0001111,//Convenience entry that represents any gameplay button
	Taunt	= 0b0010000,
	Pause	= 0b0100000,
	VE_BITMASK_VALUE_ANY
};
VE_DECLARE_BITMASK_ENUM(InputButton);

enum class InputDirection
{
	VE_BITMASK_VALUE_NONE,
	Up		= 0b0001,
	Down	= 0b0010,
	Right	= 0b0100,
	Left	= 0b1000,
	VE_BITMASK_VALUE_ANY
};
VE_DECLARE_BITMASK_ENUM(InputDirection);

enum class InputTypeMask
{
	VE_BITMASK_VALUE_NONE,
	Pressed = 0b0001,
	Released = 0b0010,
	Event = 0b0100,
	Held = 0b1000
};

VE_DECLARE_BITMASK_ENUM(InputTypeMask);