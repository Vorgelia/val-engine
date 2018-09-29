#include "Transform.h"
#include "JSON.h"
#include "BehaviourFactory.h"

ve::mat4 Transform::GetMatrix() const
{
	if(int(_updateFlags | UpdateFlags::Matrix) != 0)
	{
		_updateFlags &= ~UpdateFlags::Matrix;

		const ve::mat4 translationMat = glm::translate(ve::mat4(), ve::vec3(float(_position.x), float(_position.y), -1.0 + 1.0 / (1.0 + glm::abs(float(_depth)))));
		const ve::mat4 rotationMat = glm::mat4_cast(GetQuat());
		const ve::mat4 scaleMat = glm::scale(ve::mat4(), ve::vec3(this->_scale.x, this->_scale.y, 1));
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
void Transform::SetDepth(ve::dec_t depth)
{
	_depth = std::move(depth);
	_updateFlags |= UpdateFlags::Matrix;
}

Transform Transform::operator*(const Transform& rhs) const
{
	const ve::vec4 newPosition = rhs.GetQuat() * ve::vec4(rhs.GetScale() * GetPosition(), 1) + ve::vec4(rhs.GetPosition(), 1);
	return Transform(newPosition, rhs.GetRotation() + GetRotation(), rhs.GetScale() * GetScale());
}

Transform::Transform(ve::vec3 position, ve::vec3 eulerRotation, ve::vec3 scale)
	: _updateFlags(UpdateFlags::All)
	, _position(position)
	, _scale(scale)
	, _rotation(eulerRotation)
	, _depth(0.0f)
{

}
