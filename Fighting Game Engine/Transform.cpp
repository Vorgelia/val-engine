#include "Transform.h"
#include "JSON.h"

ve::mat4 Transform::GetMatrix() const
{
	if(int(_updateFlags | UpdateFlags::Matrix) != 0)
	{
		_updateFlags &= ~UpdateFlags::Matrix;

		const ve::mat4 translationMat = glm::translate(ve::mat4(), _position);
		const ve::mat4 rotationMat = glm::mat4_cast(GetQuat());
		const ve::mat4 scaleMat = glm::scale(ve::mat4(), _scale);
		_matrix = translationMat * rotationMat * scaleMat;
	}

	return _matrix;
}

ve::quat Transform::GetQuat() const
{
	if(int(_updateFlags | UpdateFlags::Quat) != 0)
	{
		_updateFlags &= ~UpdateFlags::Quat;
		_quat = glm::quat(glm::radians(_rotation));
	}

	return _quat;
}

void Transform::SetPosition(ve::vec3 position)
{
	_position = std::move(position);
	_updateFlags |= UpdateFlags::Matrix;
}

void Transform::SetScale(ve::vec3 scale)
{
	_scale = std::move(scale);
	_updateFlags |= UpdateFlags::Matrix;
}

void Transform::SetRotation(ve::vec3 rotation)
{
	_rotation = std::move(rotation);
	_updateFlags |= UpdateFlags::Matrix | UpdateFlags::Quat;
}

ve::vec3 Transform::GetScaleInverse() const
{
	ve::vec3 invScale{};
	for(int i = 0; i < 3; ++i)
	{
		invScale[i] = glm::nearlyZero(_scale[i]) ? 0 : (ve::dec_t(1.0) / _scale[i]);
	}
	return invScale;
}

ve::vec3 Transform::TransformLocation(const ve::vec3& location, bool applyScaling) const
{
	return TransformVector(location) + _position;
}

ve::vec3 Transform::TransformVector(const ve::vec3& vector, bool applyScaling) const
{
	return GetQuat() * ((applyScaling ? GetScale() : ve::vec3(1)) * vector);
}

ve::vec3 Transform::InverseTransformLocation(const ve::vec3& location, bool applyScaling) const
{
	return InverseTransformVector(location - _position, applyScaling);
}

ve::vec3 Transform::InverseTransformVector(const ve::vec3& vector, bool applyScaling) const
{
	return glm::inverse(GetQuat()) * vector * (applyScaling ? GetScaleInverse() : ve::vec3(1));
}

Transform Transform::GetInverse() const
{
	ve::quat invQuat = glm::inverse(GetQuat());
	ve::vec3 invScale = GetScaleInverse();
	ve::vec3 invPosition = invQuat * (invScale * - _position);

	return Transform(invPosition, invQuat, invScale);
}

Transform Transform::operator*(const Transform& rhs) const
{
	const ve::vec4 newPosition = rhs.GetQuat() * ve::vec4(rhs.GetScale() * GetPosition(), 1) + ve::vec4(rhs.GetPosition(), 1);
	return Transform(newPosition, rhs.GetRotation() + GetRotation(), rhs.GetScale() * GetScale());
}

void Transform::RegisterReflectionFields() const
{
	VE_PRIVATE_REFLECTION_VAR(Field, position);
	VE_PRIVATE_REFLECTION_VAR(Field, scale);
	VE_PRIVATE_REFLECTION_VAR(Field, rotation);
}

void Transform::Deserialize(const json& j)
{
	IReflectable::Deserialize(j);

	_updateFlags = UpdateFlags::All;
}

Transform::Transform(ve::vec3 position, ve::vec3 eulerRotation, ve::vec3 scale)
	: _updateFlags(UpdateFlags::All)
	, _position(position)
	, _scale(scale)
	, _rotation(eulerRotation)
{

}

Transform::Transform(ve::vec3 position, ve::quat quat, ve::vec3 scale)
	: _updateFlags(UpdateFlags::Matrix)
	, _position(position)
	, _scale(scale)
	, _rotation(glm::eulerAngles(quat))
	, _quat(quat)
{

}
