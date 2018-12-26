#pragma once
#include "MathIncludes.hpp"
#include "ValEngine.h"
#include "EnumUtils.h"
#include "IReflectable.h"

struct Transform : public IReflectable
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

	mutable ve::quat _quat;
	mutable ve::mat4 _matrix;

public:
	const ve::vec3& GetPosition() const { return _position; }
	const ve::vec3& GetScale() const { return _scale; }
	const ve::vec3& GetRotation() const { return _rotation; }

	ve::mat4 GetMatrix() const;
	ve::quat GetQuat() const;

	void SetPosition(ve::vec3 position);
	void SetScale(ve::vec3 scale);
	void SetRotation(ve::vec3 rotation);

	void Translate(ve::vec3 translation);
	void AddScale(ve::vec3 scale);
	void AddRotation(ve::vec3 eulerAngles);

	ve::vec3 GetScaleInverse() const;

	ve::vec3 TransformLocation(const ve::vec3& location, bool applyScaling = false) const;
	ve::vec3 TransformVector(const ve::vec3& vector, bool applyScaling = false) const;

	ve::vec3 InverseTransformLocation(const ve::vec3& location, bool applyScaling = false) const;
	ve::vec3 InverseTransformVector(const ve::vec3& vector, bool applyScaling = false) const;

	Transform GetInverse() const;

	Transform operator*(const Transform& rhs) const;

	virtual void RegisterReflectionFields() const override;
	void Deserialize(const json& j) override;

	Transform(ve::vec3 position = ve::vec3(0,0,0), ve::vec3 eulerRotation = ve::vec3(0,0,0), ve::vec3 scale = ve::vec3(1,1,1));
	Transform(ve::vec3 position, ve::quat quat, ve::vec3 scale);
};

VE_DECLARE_BITMASK_ENUM(Transform::UpdateFlags)
