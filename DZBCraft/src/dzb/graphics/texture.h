#pragma once

#include <GLEW/glew.h>
#include <FreeImage.h>
#include <string>
#include <iostream>


namespace DZB {

	class Texture
	{
	private:
		std::string m_FileName;
		GLuint m_TextureID, m_Filtering;
		GLsizei m_Width, m_Height;
		unsigned int m_BPP;
	public:
		Texture(const std::string& filename, const GLuint filtering);
		~Texture();

		void bind() const;
		void unbind() const;

		inline const unsigned int getID() const { return m_TextureID; }
		inline const unsigned int getWidth() const { return m_Width; }
		inline const unsigned int getHeight() const { return m_Height; }
	private:
		GLuint load();
	};

}