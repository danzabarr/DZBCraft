#include "camera.h"
#include "frustumculling.h"

namespace DZB
{
	Camera::Camera(glm::vec3 position, float pitch, float yaw, float fov, float zNear, float zFar) :
		position(position),
		pitch(pitch), yaw(yaw),
		fov(fov), zNear(zNear), zFar(zFar),
		frustum(new glm::vec4[6])
	{
		proj = glm::perspective(fov, (float)Screen::GetWidth() / (float)Screen::GetHeight(), zNear, zFar);
		RecalculateMatrices();
	}

	Camera::~Camera()
	{
		delete[] frustum;
	}

	void Camera::RecalculateMatrices()
	{
		transform = glm::translate(glm::mat4(1), position);
		transform = glm::rotate(transform, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, pitch, glm::vec3(1.0f, 0.0f, 0.0f));

		view = glm::inverse(transform);
		projView = proj * view;
		
		FrustumCulling::GetPlanes(projView, frustum);
	}

	bool Camera::CullBlock(int x, int y, int z)
	{
		return FrustumCulling::CullBlock(frustum, x, y, z);
	}

	bool Camera::CullChunk(int x, int z)
	{
		return FrustumCulling::CullChunk(frustum, x, z);
	}

	void Camera::ResizeCallback(GLFWwindow* window, int width, int height) 
	{
		proj = glm::perspective(fov, (float)Screen::GetWidth() / (float)Screen::GetHeight(), zNear, zFar);
	}
}