#pragma once

enum class InputButton
{
	Light = 1,
	Medium = 2,
	Heavy = 4,
	Attacks = 7,//Convenience entry that represents any of the buttons
	Shield = 8,
	Any = 15,//Convenience entry that represents any button
	Taunt = 16,
	Pause = 32
};

enum class InputDirection
{
	Up = 1,
	Down = 2,
	Right = 4,
	Left = 8
};

//Bitmask, even though it might not look like it.
enum class InputTypeMask
{
	Pressed = 1,
	Released = 2,
	Event = 3,
	Held = 4
};