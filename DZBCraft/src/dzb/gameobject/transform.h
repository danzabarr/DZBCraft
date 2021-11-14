#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtx/quaternion.hpp>
#include <vector>

namespace DZB
{
	class Transform
	{
	private:
		Transform* parent;
		std::vector<Transform*> children;

		glm::vec3 worldPosition;
		glm::quat worldRotation;
		glm::vec3 worldScale;

		glm::vec3 localPosition;
		glm::quat localRotation;
		glm::vec3 localScale;

	public: //3D
		Transform();
		Transform(Transform* parent);
		Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale, Transform* parent);

		Transform Parent() { return parent; }
		void SetParent(Transform parent, bool setRelative);
		Transform Root();
		void DetachChildren();
		std::vector<Transform*> Children();
		int ChildCount();
		Transform* Child(int n);
		Transform* FirstChild();
		Transform* LastChild();
		int IndexOf(Transform* child);
		bool ChildOf(Transform* parent);
		bool DirectChildOf(Transform* parent);
		bool ParentOf(Transform* child);
		bool DirectParentOf(Transform* child);

		glm::vec3 Position() { return worldPosition; }
		void SetPosition(glm::vec3& position);
		void SetPosition(float x, float y, float z);
		
		glm::vec3 LocalPosition() { return localPosition; }
		void SetLocalPosition(glm::vec3& position);
		void SetLocalPosition(float x, float y, float z);

		glm::quat Rotation() { return worldRotation; }
		void SetRotation(glm::quat& rotation);
		void SetRotation(float x, float y, float z);
		void SetRotation(float angle, glm::vec3& axis);

		glm::quat LocalRotation() { return localRotation; }
		void SetLocalRotation(glm::quat& rotation);
		void SetLocalRotation(float x, float y, float z);
		void SetLocalRotation(float angle, glm::vec3& axis);

		glm::vec3 Scale() { return worldScale; }
		void SetScale(glm::vec3& scale);
		void SetScale(float x, float y, float z);

		glm::vec3 LocalScale() { return localScale; }
		void SetLocalScale(glm::vec3& scale);
		void SetLocalScale(float x, float y, float z);

		void Rotate(float angle, glm::vec3& axis);
		void Rotate(float x, float y, float z);

		void RotateLocal(float angle, glm::vec3& axis);
		void RotateLocal(float x, float y, float z);

		void LookAt(glm::vec3& target, glm::vec3& up);
		void RotateAround(glm::vec3& point, glm::vec3& axis, float angle);

		void TransformDirection(glm::vec3& direction);
		void TransformPoint(glm::vec3& point);
		void TransformVector(glm::vec3& vector);

		void InverseTransformDirection(glm::vec3& direction);
		void InverseTransformPoint(glm::vec3& point);
		void InverseTransformVector(glm::vec3& vector);

		void Translate(glm::vec3& translation);
		void TranslateLocal(glm::vec3& translation);

		glm::vec3 EulerAngles();

		glm::mat4 WorldToLocalMatrix();
		glm::mat4 LocalToWorldMatrix();

		glm::vec3 Forward();
		glm::vec3 Up();
		glm::vec3 Right();

	};
}