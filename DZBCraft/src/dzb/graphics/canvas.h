#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "../app/screen.h"
#include "shader.h"
#include "texture.h"
#include <string>


#define SPRITE_MAX_COUNT				10000
#define SPRITE_VERTEX_SIZE				sizeof(SpriteVertexData)
#define SPRITE_SIZE						SPRITE_VERTEX_SIZE * 4
#define SPRITE_BUFFER_SIZE				SPRITE_SIZE * SPRITE_MAX_COUNT
#define SPRITE_INDICES_SIZE				SPRITE_MAX_COUNT * 6

#define SPRITE_SHADER_VERTEX_INDEX		0
#define SPRITE_SHADER_UV_INDEX			1

namespace DZB
{
	struct SpriteVertexData
	{
		glm::vec3 position;
		glm::vec2 uv;
	};

	class Canvas : public ScreenResizeListener
	{
	private:
		glm::mat4 m_ProjectionMatrix;
		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_IBO;
		GLuint m_IndexCount;
		SpriteVertexData* m_Buffer;
		Texture* m_CurrentTexture;
		Shader* m_SpriteShader;
	public:
		Canvas(Shader* shader);
		~Canvas();
	private:
		void ResizeCallback(GLFWwindow* window, int width, int height) override;
		void CalculateProjectionMatrix(int width, int height);
	public:
		void Begin();
		void End();

		void DrawSprite(Texture* texture, glm::vec3 position, glm::vec2 size);
		void DrawString(std::string text, glm::vec3& position);
	};
}