#pragma once

#include "renderer.h"
#include "texture.h"
#include "shader.h"
#include "indexbuffer.h"

namespace DZB
{

#define BLOCK_RENDERER_MAX_FACES	100000
#define BLOCK_RENDERER_VERTEX_SIZE	sizeof(BlockRendererVertexData)
#define BLOCK_RENDERER_FACE_SIZE	BLOCK_RENDERER_VERTEX_SIZE * 4
#define BLOCK_RENDERER_BUFFER_SIZE	BLOCK_RENDERER_FACE_SIZE * BLOCK_RENDERER_MAX_FACES
#define BLOCK_RENDERER_INDICES_SIZE	BLOCK_RENDERER_MAX_FACES * 6

#define BLOCK_SHADER_VERTEX_INDEX		0
#define BLOCK_SHADER_UV_INDEX			1

	struct BlockRendererVertexData
	{
		glm::vec3 position;
		glm::vec2 uv;
	};

	class BlockRenderer : public Renderer
	{
	private:
		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_IBO;
		GLuint m_IndexCount;
		BlockRendererVertexData* m_Buffer;
		Texture* m_BlockTexture;
		Shader* m_BlockShader;
	private:
		void AddToBuffer(unsigned int textureID, int x, int y, int z, int u, int v);
	public:
		BlockRenderer(Shader* shader, Texture* texture);
		~BlockRenderer();

		void Begin() override;
		void End() override;
		void Flush() override;

		void DrawTopFace(unsigned int textureID, int x, int y, int z);
		void DrawBottomFace(unsigned int textureID, int x, int y, int z);
		void DrawNorthFace(unsigned int textureID, int x, int y, int z);
		void DrawEastFace(unsigned int textureID, int x, int y, int z);
		void DrawSouthFace(unsigned int textureID, int x, int y, int z);
		void DrawWestFace(unsigned int textureID, int x, int y, int z);

		inline void DrawCube(unsigned int textureID, int x, int y, int z)
		{
			DrawTopFace(textureID, x, y, z);
			DrawBottomFace(textureID, x, y, z);
			DrawNorthFace(textureID, x, y, z);
			DrawEastFace(textureID, x, y, z);
			DrawSouthFace(textureID, x, y, z);
			DrawWestFace(textureID, x, y, z);
		}

		inline void StartNewBuffer()
		{
			End();
			Flush();
			Begin();
		}
	};
}