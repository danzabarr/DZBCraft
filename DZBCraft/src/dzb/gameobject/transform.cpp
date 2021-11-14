#include "transform.h"

namespace DZB
{
	Transform::Transform() :
		worldPosition(glm::vec3()),
		worldRotation(glm::quat()),
		worldScale(glm::vec3(1))
	{}

	Transform::Transform(Transform* parent) :
		worldPosition(glm::vec3()),
		worldRotation(glm::quat()),
		worldScale(glm::vec3(1)),
		parent(parent)
	{}

	Transform::Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale, Transform* parent) :
		worldPosition(position),
		worldRotation(rotation),
		worldScale(scale),
		parent(parent)
	{}

	Transform Transform::Root()
	{
		if (parent == nullptr) return this;
		return parent->Root();
	}
	/*
	

	void Transform::SetParent(Transform parent, bool setRelative)
	{
	}

	void Transform::DetachChildren()
	{
	}

	std::vector<Transform*> Transform::Children()
	{
	}

	int Transform::ChildCount()
	{
	}

	Transform* Transform::Child(int n)
	{
	}

	Transform* Transform::FirstChild()
	{
	}

	Transform* Transform::LastChild()
	{
	}

	int Transform::IndexOf(Transform* child)
	{
	}

	bool Transform::ChildOf(Transform* parent)
	{
	}

	bool Transform::DirectChildOf(Transform* parent)
	{
	}

	bool Transform::ParentOf(Transform* child)
	{
	}

	bool Transform::DirectParentOf(Transform* child)
	{
	}

	void Transform::SetPosition(glm::vec3& position)
	{
	}

	void Transform::SetPosition(float x, float y, float z)
	{
	}

	void Transform::SetLocalPosition(glm::vec3& position)
	{
	}

	void Transform::SetLocalPosition(float x, float y, float z)
	{
	}

	void Transform::SetRotation(glm::quat& rotation) 
	{
	}

	void Transform::SetRotation(float x, float y, float z) 
	{
	}

	void Transform::SetRotation(float angle, glm::vec3& axis) 
	{
	}

	void Transform::SetLocalRotation(glm::quat& rotation) 
	{
	}

	void Transform::SetLocalRotation(float x, float y, float z) 
	{
	}

	void Transform::SetLocalRotation(float angle, glm::vec3& axis) 
	{
	}

	void Transform::SetScale(glm::vec3& scale) 
	{
	}

	void Transform::SetScale(float x, float y, float z) 
	{
	}

	void Transform::SetLocalScale(glm::vec3& scale) 
	{
	}

	void Transform::SetLocalScale(float x, float y, float z) 
	{
	}

	void Transform::Rotate(float angle, glm::vec3& axis) 
	{
	}

	void Transform::Rotate(float x, float y, float z) 
	{
	}

	void Transform::RotateLocal(float angle, glm::vec3& axis) 
	{
	}

	void Transform::RotateLocal(float x, float y, float z) 
	{
	}

	void Transform::LookAt(glm::vec3& target, glm::vec3& up) 
	{
	}

	void Transform::RotateAround(glm::vec3& point, glm::vec3& axis, float angle) 
	{
	}

	void Transform::TransformDirection(glm::vec3& direction) 
	{
	}

	void Transform::TransformPoint(glm::vec3& point) 
	{
	}

	void Transform::TransformVector(glm::vec3& vector) 
	{
	}

	void Transform::InverseTransformDirection(glm::vec3& direction) 
	{
	}

	void Transform::InverseTransformPoint(glm::vec3& point) 
	{
	}

	void Transform::InverseTransformVector(glm::vec3& vector) 
	{
	}

	void Transform::Translate(glm::vec3& translation) 
	{
	}

	void Transform::TranslateLocal(glm::vec3& translation) 
	{
	}

	glm::vec3 Transform::EulerAngles() 
	{
	}

	glm::mat4 Transform::WorldToLocalMatrix() 
	{
	}

	glm::mat4 Transform::LocalToWorldMatrix() 
	{
	}

	glm::vec3 Transform::Forward() 
	{
	}

	glm::vec3 Transform::Up() 
	{
	}

	glm::vec3 Transform::Right() 
	{
	}
*/

}