#include "texture.h"
#include "../utils/imageloader.h"

namespace DZB {

	Texture::Texture(const std::string& filename, const GLuint filtering)
		: m_FileName(filename), m_Filtering(filtering)
	{
		m_TextureID = load();
	}

	Texture::~Texture()
	{

	}

	GLuint Texture::load()
	{
		BYTE* pixels = LoadImage(m_FileName.c_str(), &m_Width, &m_Height, &m_BPP);

		if (pixels == nullptr)
		{
			std::cout << "Failed to load image from " << m_FileName << ": File not found" << std::endl;
		}

		GLuint id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Filtering);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Filtering);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (m_BPP == 24)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);

		else if (m_BPP == 32)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

		else
		{
			std::cout << "Failed to load image from " << m_FileName << ": Unsupported bit depth " << m_BPP << std::endl;
			return 0;
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		//delete[] pixels;

		return id;
	}

	void Texture::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	void Texture::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}