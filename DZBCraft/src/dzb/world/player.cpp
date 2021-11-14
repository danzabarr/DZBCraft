#include "player.h"
#include <GLM/gtc/constants.hpp>
#include "../defines.h"

namespace DZB
{
	Player::Player(const glm::vec3& position, float fov, float zNear, float zFar, const glm::vec2& mouseSensitivity) :
		position(position),
		camera(new Camera(glm::vec3(position.x, position.y - PLAYER_EYES_HEIGHT, position.z), 0, 0, fov, zNear, zFar)),
		mouseSensitivity(mouseSensitivity),
		mouse(Input::MousePosition())
	{ }

	void Player::Update()
	{
		MovementControls();
		MouseButtonControls();
	}

	bool Player::LegalPosition(float x, float y, float z)
	{
		float halfSize =0.4f;
		float height = 1.8f;

		int minX = floor(x - halfSize);
		int minY = floor(y - height);
		int minZ = floor(z - halfSize);

		if (World::GetInstance().BlockType(minX, minY, minZ) != 0) return false;

		int maxX = minX;
		int maxY = minY;
		int maxZ = minZ;

		for (int i = 0; i < 3; i++)
		{
			if (x + halfSize < maxX + 1) break;
			maxX++;
			if (World::GetInstance().BlockType(maxX, minY, minZ) != 0) return false;
		}
		for (int i = 0; i < 3; i++)
		{
			if (y < maxY + 1) break;
			maxY++;
			if (World::GetInstance().BlockType(minX, maxY, minZ) != 0) return false;
		}
		for (int i = 0; i < 3; i++)
		{
			if (z + halfSize < maxZ + 1) break;
			maxZ++;
			if (World::GetInstance().BlockType(minX, minY, maxZ) != 0) return false;
		}

		for (int x = minX; x <= maxX; x++)
			for (int y = minY; y <= maxY; y++)
				for (int z = minZ; z <= maxZ; z++)
				{
					if (World::GetInstance().BlockType(x, y, z) != 0) return false;
				}
		return true;
	}

	void Player::MovementControls()
	{
		glm::vec2 currentPosition = Input::MousePosition();

		camera->yaw += (currentPosition.x - mouse.x) * mouseSensitivity.x;
		camera->pitch += (currentPosition.y - mouse.y) * mouseSensitivity.y;
		camera->pitch = glm::clamp(camera->pitch, -HALF_PI, HALF_PI);

		float pitch = camera->pitch;
		float yaw = camera->yaw;

		mouse = currentPosition;

		float movementSpeed = PLAYER_WALK_SPEED;

		glm::vec2 keyMovement(0);

		if (Input::IsKeyDown(GLFW_KEY_A))
		{
			keyMovement.x += 1;
		}

		if (Input::IsKeyDown(GLFW_KEY_D))
		{
			keyMovement.x -= 1;
		}

		if (Input::IsKeyDown(GLFW_KEY_W))
		{
			keyMovement.y -= 1;
		}
		else if (Input::IsKeyDown(GLFW_KEY_S))
		{
			keyMovement.y += 1;
			movementSpeed *= PLAYER_BACKPEDAL_MUL;
		}

		glm::vec3 forward(
			glm::sin(yaw),
			0,
			glm::cos(yaw)
		);

		glm::vec3 right(
			glm::cos(yaw),
			0,
			-glm::sin(yaw)
		);

		if (Input::IsKeyDown(GLFW_KEY_LEFT_CONTROL))
		{
			if (keyMovement.x != 0 || keyMovement.y != 0)
			{
				glm::vec2 xzMove = glm::normalize(keyMovement) * movementSpeed;
				velocity += right * xzMove.x + forward * xzMove.y;
			}

			if (Input::IsKeyDown(GLFW_KEY_SPACE))
				velocity.y -= PLAYER_FLY_UP_SPEED;

			if (Input::IsKeyDown(GLFW_KEY_X))
				velocity.y += PLAYER_FLY_DOWN_SPEED;

			position += velocity;

			velocity *= 0.8f;
		}
		else
		{
			//TODO: collision test
			bool onFloor = !LegalPosition(position.x, position.y + 0.1f, position.z);

			if (onFloor)
			{
				jumpTimer--;

				if (Input::IsKeyDown(GLFW_KEY_SPACE) && jumpTimer <= 0)
				{
					jumpTimer = PLAYER_JUMP_DELAY;
					glm::vec3 xyzMove(0, -1, 0);
					if (keyMovement.x != 0 || keyMovement.y != 0)
					{
						glm::vec2 xzMove = glm::normalize(keyMovement);
						xyzMove += (right * xzMove.x + forward * xzMove.y) * 0.5f;
					}
					
					xyzMove = glm::normalize(xyzMove) * PLAYER_JUMP_VELOCITY;
					velocity += xyzMove;
				}
				else {
					if (keyMovement.x != 0 || keyMovement.y != 0)
					{
						glm::vec2 xzMove = glm::normalize(keyMovement) * movementSpeed;
						velocity += right * xzMove.x + forward * xzMove.y;
					}
				}

				velocity *= 0.5f;
			}
			else
			{
				velocity.y += GRAVITY;
				velocity *= 0.9f;
			}

			if (LegalPosition(position.x, position.y + velocity.y, position.z))
				position.y += velocity.y;
			else velocity.y *= 0.1f;

			if (LegalPosition(position.x + velocity.x, position.y, position.z))
				position.x += velocity.x;
			else velocity.x *= 0.1f;

			if (LegalPosition(position.x, position.y, position.z + velocity.z))
				position.z += velocity.z;
			else velocity.z *= 0.1f;
		}

		camera->position.x = position.x;
		camera->position.y = position.y - PLAYER_EYES_HEIGHT;
		camera->position.z = position.z;

		camera->RecalculateMatrices();
	}

	void Player::MouseButtonControls()
	{
		//TODO: Move to Player
		glm::vec3 direction = -GetCamera()->Forward();
		glm::vec3 eyepos = EyePos() - direction * 0.1f;


		blockInSights = World::GetInstance().RaycastBlock(eyepos, direction, 5, 10, raycastHit);
		if (blockInSights)
		{
			breakPos = raycastHit.BreakPosition();
			placePos = raycastHit.PlacePosition();
		}

		if (Input::IsButtonDown(0))
		{
			if (lClickLatch)
			{
				lClickLatch = false;
				if (blockInSights)
				{
					World::GetInstance().BreakBlock(raycastHit);

					blockInSights = World::GetInstance().RaycastBlock(eyepos, direction, 5, 10, raycastHit);
					if (blockInSights)
					{
						breakPos = raycastHit.BreakPosition();
						placePos = raycastHit.PlacePosition();
					}
				}
			}
		}
		else lClickLatch = true;


		if (Input::IsButtonDown(1))
		{
			if (rClickLatch)
			{
				rClickLatch = false;
				if (blockInSights)
				{
					World::GetInstance().PlaceBlock(heldBlock, raycastHit);

					blockInSights = World::GetInstance().RaycastBlock(eyepos, direction, 5, 10, raycastHit);
					if (blockInSights)
					{
						breakPos = raycastHit.BreakPosition();
						placePos = raycastHit.PlacePosition();
					}
				}
			}
		}
		else rClickLatch = true;
	
		if (Input::IsKeyDown(GLFW_KEY_RIGHT))
		{
			if (nextBlockLatch)
			{
				nextBlockLatch = false;
				heldBlock++;
				std::cout << int(heldBlock) << std::endl;
			}
		}
		else nextBlockLatch = true;

		if (Input::IsKeyDown(GLFW_KEY_LEFT))
		{
			if (prevBlockLatch)
			{
				prevBlockLatch = false;
				heldBlock--;
				std::cout << int(heldBlock) << std::endl;
			}
		}
		else prevBlockLatch = true;

	}
}