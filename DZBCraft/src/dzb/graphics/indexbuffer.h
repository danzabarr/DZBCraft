#pragma once

#include <GLEW/glew.h>
#include <GLM/glm.hpp>

namespace DZB {

	class IndexBuffer
	{
	private:
		GLuint m_BufferID;
		GLuint m_Count;
	public:
		IndexBuffer(GLuint* data, GLsizei count);
		~IndexBuffer();

		void bind() const;
		void unbind() const;

		GLuint getCount() const { return m_Count; }
	};
}