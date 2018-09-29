#pragma once
#include "MathIncludes.hpp"
#include "ValEngine.h"
#include "EnumUtils.h"

struct Transform
{
public:
	enum class UpdateFlags : std::uint8_t
	{
		VE_BITMASK_VALUE_NONE,
		Matrix = 0b01,
		Quat = 0b10,
		VE_BITMASK_VALUE_ALL
	};

private:
	mutable UpdateFlags _updateFlags;

protected:
	ve::vec3 _position;
	ve::vec3 _scale;
	ve::vec3 _rotation;
	ve::dec_t _depth;

	mutable ve::quat _quat;
	mutable ve::mat4 _matrix;

public:
	const ve::vec3& GetPosition() const { return _position; }
	const ve::vec3& GetScale() const { return _scale; }
	const ve::vec3& GetRotation() const { return _rotation; }
	ve::dec_t GetDepth() const { return _depth; }

	ve::mat4 GetMatrix() const;
	ve::quat GetQuat() const;

	void SetPosition(ve::vec3 position);
	void SetScale(ve::vec3 scale);
	void SetRotation(ve::vec3 rotation);
	void SetDepth(ve::dec_t depth);

	Transform operator*(const Transform& rhs) const;

	Transform(ve::vec3 position, ve::vec3 eulerRotation, ve::vec3 scale);
};

VE_DECLARE_BITMASK_ENUM(Transform::UpdateFlags)
