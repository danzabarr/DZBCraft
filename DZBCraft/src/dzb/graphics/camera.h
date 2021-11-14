#pragma once
#include <GLEW/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "../app/screen.h"
#include "../defines.h"

namespace DZB
{
	class Camera : public ScreenResizeListener
	{
	public:
		Camera(glm::vec3 position, float pitch, float yaw, float fov, float zNear, float zFar);
		~Camera();
	public:
		glm::vec3 position;
		float pitch;
		float yaw;
		float fov;
		float zNear;
		float zFar;
	private:
		glm::mat4 transform;
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 projView;
		glm::vec4* frustum;
	public:
		glm::mat4 GetTransformationMatrix() { return transform; }
		glm::mat4 GetProjectionMatrix() { return proj; }
		glm::mat4 GetViewMatrix() { return view; }
		glm::mat4 GetProjectionViewMatrix() { return projView; }
		glm::vec4* GetFrustum() { return frustum; }

		glm::vec3 Forward()
		{
			return glm::vec3
			(
				/*
				glm::cos(pitch) * glm::cos(yaw),
				glm::sin(pitch),
				glm::cos(pitch) * glm::sin(yaw)
				*/

				glm::sin(yaw) * glm::cos(pitch),
				-glm::sin(pitch),
				glm::cos(pitch) * glm::cos(yaw)
			);
		}
		glm::vec3 Right()
		{
			return glm::vec3
			(
				glm::cos(yaw),
				0,
				-glm::sin(yaw)
			);
		}

		glm::vec3 Up()
		{
			return glm::vec3
			(
				glm::sin(yaw) * glm::cos(pitch + HALF_PI),
				-glm::sin(pitch + HALF_PI),
				glm::cos(pitch + HALF_PI) * glm::cos(yaw)
			);
		}

	public:
		void RecalculateMatrices();
		bool CullBlock(int x, int y, int z);
		bool CullChunk(int x, int z);
	private:
		void ResizeCallback(GLFWwindow* window, int width, int height) override;
	};
}