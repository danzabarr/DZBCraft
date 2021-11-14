#include "canvas.h"

namespace DZB
{
	Canvas::Canvas(Shader* shader) :
		m_SpriteShader(shader)
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, SPRITE_BUFFER_SIZE, NULL, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SPRITE_VERTEX_SIZE, (const GLvoid*)(offsetof(SpriteVertexData, SpriteVertexData::position)));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, SPRITE_VERTEX_SIZE, (const GLvoid*)(offsetof(SpriteVertexData, SpriteVertexData::uv)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint* indices = new GLuint[SPRITE_INDICES_SIZE];

		int offset = 0;
		for (int i = 0; i < SPRITE_INDICES_SIZE; i += 6)
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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, SPRITE_INDICES_SIZE * sizeof(GLuint), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		delete[] indices;

		glBindVertexArray(0);

		CalculateProjectionMatrix(Screen::GetWidth(), Screen::GetHeight());
	}

	Canvas::~Canvas()
	{
		glDeleteBuffers(1, &m_IBO);
		glDeleteBuffers(1, &m_VBO);
	}

	void Canvas::CalculateProjectionMatrix(int width, int height)
	{
		//m_ProjectionMatrix = glm::ortho(-(float)width / 2, (float)width / 2, -(float)height / 2, (float)height / 2);
		m_ProjectionMatrix = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
		m_SpriteShader->enable();
		m_SpriteShader->setUniformMat4("pr_matrix", m_ProjectionMatrix);
	}

	void Canvas::ResizeCallback(GLFWwindow* window, int width, int height)
	{
		CalculateProjectionMatrix(width, height);
	}

	void Canvas::Begin()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		m_Buffer = (SpriteVertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		m_IndexCount = 0;

		m_CurrentTexture = nullptr;

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		m_SpriteShader->enable();
	}

	void Canvas::DrawSprite(Texture* texture, glm::vec3 position, glm::vec2 size)
	{
		if (texture != m_CurrentTexture)
		{
			
			if (m_IndexCount > 0)
			{
				glUnmapBuffer(GL_ARRAY_BUFFER);

				glBindVertexArray(m_VAO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
				glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, NULL);

				glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
				m_Buffer = (SpriteVertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
				m_IndexCount = 0;
			}
			
			m_CurrentTexture = texture;
			glActiveTexture(GL_TEXTURE0);
			m_CurrentTexture->bind();
		}

		m_Buffer->position = glm::vec3(position.x, position.y, position.z);
		m_Buffer->uv = glm::vec2(0, 0);
		m_Buffer++;

		m_Buffer->position = glm::vec3(position.x, position.y + size.y, position.z);
		m_Buffer->uv = glm::vec2(0, 1);
		m_Buffer++;

		m_Buffer->position = glm::vec3(position.x + size.x, position.y + size.y, position.z);
		m_Buffer->uv = glm::vec2(1, 1);
		m_Buffer++;

		m_Buffer->position = glm::vec3(position.x + size.x, position.y, position.z);
		m_Buffer->uv = glm::vec2(0, 1);
		m_Buffer++;

		m_IndexCount += 6;
	}

	void Canvas::End()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, NULL);

		//m_IBO->unbind();
		//glBindVertexArray(0);

		m_IndexCount = 0;

	}
}