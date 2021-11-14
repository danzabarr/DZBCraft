#pragma once
#include <GLEW/glew.h>
#include <GLM/glm.hpp>
#include "../graphics/camera.h"
#include "world.h"

namespace DZB
{


	class Player
	{
	public:
		Player(const glm::vec3& position, float fov, float zNear, float zFar, const glm::vec2& mouseSensitivity);
	private:
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec2 mouseSensitivity;
		Camera* camera;
		glm::vec2 mouse;
	public:
		Camera* GetCamera() const { return camera; }
		glm::vec3 FeetPos() const { return position; }
		glm::vec3 EyePos() const { return camera->position; }
		glm::vec3 HeadPos() const { return glm::vec3(position.x, position.y - PLAYER_HEAD_HEIGHT, position.z); }
	public:
		void Update();

	private:
		void MovementControls();
		void MouseButtonControls();
		bool LegalPosition(float moveX, float moveY, float moveZ);

	private:
		bool lClickLatch;
		bool rClickLatch;
		bool nextBlockLatch;
		bool prevBlockLatch;
		float jumpTimer;

		RaycastHit raycastHit;
		bool blockInSights;
		bType heldBlock;

		glm::vec3 breakPos;
		glm::vec3 placePos;

	public:
		inline const bool IsBlockInSights() { return blockInSights; }
		inline const RaycastHit RaycastHit() { return raycastHit; }
		inline const glm::vec3 BreakPosition() { return breakPos; }
		inline const glm::vec3 PlacePosition() { return placePos; }
	};
}