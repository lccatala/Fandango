#include "fndgpch.h"
#include "OpenGLTexture2D.h"
#include "stb_image.h"

#include <glad/glad.h>


namespace Fandango
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		// Read texture data from file
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1); // Flip the image (opengl expects it bottom-to-top)
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		FNDG_ENGINE_ASSERT(data, "Failed to load image, {0}", path);
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		FNDG_ENGINE_ASSERT(internalFormat & dataFormat, "Texture format not supported");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		// Use linear filtering when applying texture to a bigger object
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Take closest pixel's color when applying texture to a smaller object
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Upload texture to GPU
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		// Once uploaded to GPU, free it form memory
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const 
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}