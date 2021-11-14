#pragma once

#include <GLEW/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>


namespace DZB {

	class Shader
	{
	private:
		GLuint m_ShaderID;
		const char* m_VertPath;
		const char* m_FragPath;
	public:
		Shader(const char* vertPath, const char* fragPath);
		~Shader();

		void setUniform1i(const GLchar* name, int value);
		void setUniform1iv(const GLchar* name, int count, int* value);
		void setUniform1f(const GLchar* name, float value);
		void setUniform2f(const GLchar* name, const glm::vec2& vector);
		void setUniform3f(const GLchar* name, const glm::vec3& vector);
		void setUniform4f(const GLchar* name, const glm::vec4& vector);
		void setUniformMat4(const GLchar* name, const glm::mat4& matrix);

		void enable() const;
		void disable() const;
	private:
		GLint getUniformLocation(const GLchar* name);
		GLuint load();
	};

}