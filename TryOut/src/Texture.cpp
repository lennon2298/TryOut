#include <iostream>
#include "Texture.h"

Texture::Texture()
	: m_Width(0), m_Height(0), m_Internal_Format(GL_RGB), m_Image_Format(GL_RGB), m_Wrap_S(GL_CLAMP_TO_BORDER), m_Wrap_T(GL_CLAMP_TO_BORDER), m_Filter_Min(GL_LINEAR), m_Filter_Max(GL_LINEAR)
{
	GLCall(glGenTextures(1, &this->m_ID));
}

Texture::~Texture()
{
}

void Texture::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
	this->m_Width = width;
	this->m_Height = height;
	GLCall(glBindTexture(GL_TEXTURE_2D, this->m_ID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Wrap_S));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Wrap_T));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Filter_Min));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Filter_Max));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, this->m_Internal_Format, this->m_Width, this->m_Height, 0, this->m_Image_Format, GL_UNSIGNED_BYTE, data));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Bind() const
{
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D, this->m_ID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::SetTexWrap(unsigned int wrap_s, unsigned int wrap_t)
{
	this->m_Wrap_S = wrap_s;
	this->m_Wrap_T = wrap_t;
}

void Texture::SetMinMaxFilter(unsigned int minFilter, unsigned maxFilter)
{
	this->m_Filter_Min = minFilter;
	this->m_Filter_Max = maxFilter;
}

void Texture::SetAlphaChannel()
{
	this->m_Internal_Format = GL_RGBA;
	this->m_Image_Format = GL_RGBA;
}
