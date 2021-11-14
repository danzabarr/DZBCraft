#include "blockrenderer.h"

namespace DZB
{
	BlockRenderer::BlockRenderer(Shader* shader, Texture* texture)
		: m_BlockTexture(texture), m_BlockShader(shader), m_IndexCount(0), m_Buffer(nullptr)
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, BLOCK_RENDERER_BUFFER_SIZE, NULL, GL_STATIC_DRAW);

		glEnableVertexAttribArray(BLOCK_SHADER_VERTEX_INDEX);
		glEnableVertexAttribArray(BLOCK_SHADER_UV_INDEX);

		glVertexAttribPointer(BLOCK_SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, BLOCK_RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(BlockRendererVertexData, BlockRendererVertexData::position)));
		glVertexAttribPointer(BLOCK_SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, BLOCK_RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(BlockRendererVertexData, BlockRendererVertexData::uv)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint* indices = new GLuint[BLOCK_RENDERER_INDICES_SIZE];

		int offset = 0;
		for (int i = 0; i < BLOCK_RENDERER_INDICES_SIZE; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;
			offset += 4;
		}

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, BLOCK_RENDERER_INDICES_SIZE * sizeof(GLuint), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		delete[] indices;

		glBindVertexArray(0);

	}

	BlockRenderer::~BlockRenderer()
	{
		glDeleteBuffers(1, &m_IBO);
		glDeleteBuffers(1, &m_VBO);
	}

	void BlockRenderer::Begin()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		m_Buffer = (BlockRendererVertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		m_IndexCount = 0;
	}

	void BlockRenderer::AddToBuffer(unsigned int textureID, int x, int y, int z, int u, int v)
	{
		m_Buffer->position = glm::vec3(x, y, z);
		m_Buffer->uv = glm::vec2((u + textureID % 16) * 16.0f / 256.0f, (v + 15 - textureID / 16) * 16.0f / 256.0f);
		m_Buffer++;
	}

	void BlockRenderer::DrawTopFace(unsigned int textureID, int x, int y, int z)
	{
		if (m_IndexCount + 6 > BLOCK_RENDERER_INDICES_SIZE)
			StartNewBuffer();

		AddToBuffer(textureID, x + 0, y + 0, z + 1, 0, 0);
		AddToBuffer(textureID, x + 0, y + 0, z + 0, 0, 1);
		AddToBuffer(textureID, x + 1, y + 0, z + 0, 1, 1);
		AddToBuffer(textureID, x + 1, y + 0, z + 1, 1, 0);
		m_IndexCount += 6;
	}

	void BlockRenderer::DrawBottomFace(unsigned int textureID, int x, int y, int z)
	{
		if (m_IndexCount + 6 > BLOCK_RENDERER_INDICES_SIZE)
			StartNewBuffer();

		AddToBuffer(textureID, x + 0, y + 1, z + 0, 0, 0);
		AddToBuffer(textureID, x + 0, y + 1, z + 1, 0, 1);
		AddToBuffer(textureID, x + 1, y + 1, z + 1, 1, 1);
		AddToBuffer(textureID, x + 1, y + 1, z + 0, 1, 0);
		m_IndexCount += 6;
	}

	void BlockRenderer::DrawNorthFace(unsigned int textureID, int x, int y, int z)
	{
		if (m_IndexCount + 6 > BLOCK_RENDERER_INDICES_SIZE)
			StartNewBuffer();

		AddToBuffer(textureID, x + 0, y + 0, z + 0, 0, 0);
		AddToBuffer(textureID, x + 0, y + 1, z + 0, 0, 1);
		AddToBuffer(textureID, x + 1, y + 1, z + 0, 1, 1);
		AddToBuffer(textureID, x + 1, y + 0, z + 0, 1, 0);
		m_IndexCount += 6;
	}

	void BlockRenderer::DrawEastFace(unsigned int textureID, int x, int y, int z)
	{
		if (m_IndexCount + 6 > BLOCK_RENDERER_INDICES_SIZE)
			StartNewBuffer();

		AddToBuffer(textureID, x + 0, y + 0, z + 1, 0, 0);
		AddToBuffer(textureID, x + 0, y + 1, z + 1, 0, 1);
		AddToBuffer(textureID, x + 0, y + 1, z + 0, 1, 1);
		AddToBuffer(textureID, x + 0, y + 0, z + 0, 1, 0);
		m_IndexCount += 6;
	}

	void BlockRenderer::DrawSouthFace(unsigned int textureID, int x, int y, int z)
	{
		if (m_IndexCount + 6 > BLOCK_RENDERER_INDICES_SIZE)
			StartNewBuffer();

		AddToBuffer(textureID, x + 1, y + 0, z + 1, 0, 0);
		AddToBuffer(textureID, x + 1, y + 1, z + 1, 0, 1);
		AddToBuffer(textureID, x + 0, y + 1, z + 1, 1, 1);
		AddToBuffer(textureID, x + 0, y + 0, z + 1, 1, 0);
		m_IndexCount += 6;
	}

	void BlockRenderer::DrawWestFace(unsigned int textureID, int x, int y, int z)
	{
		if (m_IndexCount + 6 > BLOCK_RENDERER_INDICES_SIZE)
			StartNewBuffer();

		AddToBuffer(textureID, x + 1, y + 0, z + 0, 0, 0);
		AddToBuffer(textureID, x + 1, y + 1, z + 0, 0, 1);
		AddToBuffer(textureID, x + 1, y + 1, z + 1, 1, 1);
		AddToBuffer(textureID, x + 1, y + 0, z + 1, 1, 0);
		m_IndexCount += 6;
	}

	void BlockRenderer::End()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void BlockRenderer::Flush()
	{
		m_BlockShader->enable();

		glActiveTexture(GL_TEXTURE0);
		m_BlockTexture->bind();

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, NULL);

		//m_IBO->unbind();
		//glBindVertexArray(0);

		m_IndexCount = 0;

		drawCalls++;
	}
}