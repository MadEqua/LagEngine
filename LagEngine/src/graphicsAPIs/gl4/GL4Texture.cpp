#include "GL4Texture.h"

using namespace Lag;

GL4Texture::GL4Texture(const std::string &path, TextureType type, const TextureData &data) :
	Texture(path, type, data)
{
	GL_ERROR_CHECK(glCreateTextures(getGLType(), 1, &handle))
}

GL4Texture::~GL4Texture()
{
	GL_ERROR_CHECK(glDeleteTextures(1, &handle))
}

bool GL4Texture::loadImplementation()
{
	if (!loadFromFile())
		return false;

	switch (type)
	{
	case LAG_TEXTURE_TYPE_1D:
		GL_ERROR_CHECK(glTextureStorage1D(handle, data.mipmaps, getPixelDataSizedFormatGL(), width))
		GL_ERROR_CHECK(glTextureSubImage1D(handle, 0, 0, width, getPixelDataFormatGL(), getPixelDataTypeGL(), dataPtr))
		break;
	case LAG_TEXTURE_TYPE_2D:
		GL_ERROR_CHECK(glTextureStorage2D(handle, data.mipmaps, getPixelDataSizedFormatGL(), width, height))
		GL_ERROR_CHECK(glTextureSubImage2D(handle, 0, 0, 0, width, height, getPixelDataFormatGL(), getPixelDataTypeGL(), dataPtr))
		break;
	default:
		return false;
	}

	if (data.mipmaps > 1)
		GL_ERROR_CHECK(glGenerateTextureMipmap(handle))

	return true;
}

void GL4Texture::unloadImplementation()
{
}

GLenum GL4Texture::getGLType() const
{
	switch (type)
	{
	case LAG_TEXTURE_TYPE_1D:
		return GL_TEXTURE_1D;
	case LAG_TEXTURE_TYPE_2D:
		return GL_TEXTURE_2D;
	default:
		return GL_TEXTURE_2D;
	}
}

GLenum GL4Texture::getPixelDataFormatGL() const
{
	switch (data.components)
	{
	//TODO: BGRA and BGR because of FreeImage
	case LAG_TEXTURE_COMPONENTS_RGBA:
		return GL_BGRA;
	case LAG_TEXTURE_COMPONENTS_RGB:
		return GL_BGR;
	case LAG_TEXTURE_COMPONENTS_RG:
		return GL_RG;
	case LAG_TEXTURE_COMPONENTS_R:
		return GL_RED;
	default:
		return -1;
	}
}

GLenum GL4Texture::getPixelDataTypeGL() const
{
	switch (data.componentType)
	{
	case LAG_TEXTURE_COMPONENT_TYPE_FLOAT32:
		return GL_FLOAT;

	case LAG_TEXTURE_COMPONENT_TYPE_INT8:
		return GL_BYTE;
	case LAG_TEXTURE_COMPONENT_TYPE_INT16:
		return GL_SHORT;
	case LAG_TEXTURE_COMPONENT_TYPE_INT32:
		return GL_INT;

	case LAG_TEXTURE_COMPONENT_TYPE_UINT8:
		return GL_UNSIGNED_BYTE;
	case LAG_TEXTURE_COMPONENT_TYPE_UINT16:
		return GL_UNSIGNED_SHORT;
	case LAG_TEXTURE_COMPONENT_TYPE_UINT32:
		return GL_UNSIGNED_INT;

	//can't send float16 from cpu to a texture. 
	//float16 is an internal format.
	case LAG_TEXTURE_COMPONENT_TYPE_FLOAT16: 
	default:
		return -1;
	}
}

GLenum GL4Texture::getPixelDataSizedFormatGL() const
{
	switch (data.componentType)
	{
	case LAG_TEXTURE_COMPONENT_TYPE_FLOAT16:
		switch (data.components)
		{
		case LAG_TEXTURE_COMPONENTS_RGBA:
			return GL_RGBA16F;
		case LAG_TEXTURE_COMPONENTS_RGB:
			return GL_RGB16F;
		case LAG_TEXTURE_COMPONENTS_RG:
			return GL_RG16F;
		case LAG_TEXTURE_COMPONENTS_R:
			return GL_R16F;
		default:
			return -1;
		}

	case LAG_TEXTURE_COMPONENT_TYPE_FLOAT32:
		switch (data.components)
		{
		case LAG_TEXTURE_COMPONENTS_RGBA:
			return GL_RGBA32F;
		case LAG_TEXTURE_COMPONENTS_RGB:
			return GL_RGB32F;
		case LAG_TEXTURE_COMPONENTS_RG:
			return GL_RG32F;
		case LAG_TEXTURE_COMPONENTS_R:
			return GL_R32F;
		default:
			return -1;
		}

	case LAG_TEXTURE_COMPONENT_TYPE_INT8:
		switch (data.components)
		{
		case LAG_TEXTURE_COMPONENTS_RGBA:
			return data.normalized ?  GL_RGBA8_SNORM : GL_RGBA8I;
		case LAG_TEXTURE_COMPONENTS_RGB:
			return data.normalized ? GL_RGB8_SNORM : GL_RGB8I;
		case LAG_TEXTURE_COMPONENTS_RG:
			return data.normalized ? GL_RG8_SNORM : GL_RG8I;
		case LAG_TEXTURE_COMPONENTS_R:
			return data.normalized ? GL_R8_SNORM : GL_R8I;
		default:
			return -1;
		}

	case LAG_TEXTURE_COMPONENT_TYPE_INT16:
		switch (data.components)
		{
		case LAG_TEXTURE_COMPONENTS_RGBA:
			return data.normalized ? GL_RGBA16_SNORM : GL_RGBA16I;
		case LAG_TEXTURE_COMPONENTS_RGB:
			return data.normalized ? GL_RGB16_SNORM : GL_RGB16I;
		case LAG_TEXTURE_COMPONENTS_RG:
			return data.normalized ? GL_RG16_SNORM : GL_RG16I;
		case LAG_TEXTURE_COMPONENTS_R:
			return data.normalized ? GL_R16_SNORM : GL_R16I;
		default:
			return -1;
		}

	case LAG_TEXTURE_COMPONENT_TYPE_INT32:
		switch (data.components)
		{
		case LAG_TEXTURE_COMPONENTS_RGBA:
			return GL_RGBA32I;
		case LAG_TEXTURE_COMPONENTS_RGB:
			return GL_RGB32I;
		case LAG_TEXTURE_COMPONENTS_RG:
			return GL_RG32I;
		case LAG_TEXTURE_COMPONENTS_R:
			return GL_R32I;
		default:
			return -1;
		}

	case LAG_TEXTURE_COMPONENT_TYPE_UINT8:
		switch (data.components)
		{
		case LAG_TEXTURE_COMPONENTS_RGBA:
			return data.normalized ? (data.sRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8) : GL_RGBA8UI;
		case LAG_TEXTURE_COMPONENTS_RGB:
			return data.normalized ? (data.sRGB ? GL_SRGB8 : GL_RGB8) : GL_RGB8UI;
		case LAG_TEXTURE_COMPONENTS_RG:
			return data.normalized ? GL_RG8 : GL_RG8UI;
		case LAG_TEXTURE_COMPONENTS_R:
			return data.normalized ? GL_R8 : GL_R8UI;
		default:
			return -1;
		}

	case LAG_TEXTURE_COMPONENT_TYPE_UINT16:
		switch (data.components)
		{
		case LAG_TEXTURE_COMPONENTS_RGBA:
			return data.normalized ? GL_RGBA16 : GL_RGBA16UI;
		case LAG_TEXTURE_COMPONENTS_RGB:
			return data.normalized ? GL_RGB16 : GL_RGB16UI;
		case LAG_TEXTURE_COMPONENTS_RG:
			return data.normalized ? GL_RG16 : GL_RG16UI;
		case LAG_TEXTURE_COMPONENTS_R:
			return data.normalized ? GL_R16 : GL_R16UI;
		default:
			return -1;
		}
		
	case LAG_TEXTURE_COMPONENT_TYPE_UINT32:
		switch (data.components)
		{
		case LAG_TEXTURE_COMPONENTS_RGBA:
			return data.normalized ? -1 : GL_RGBA32UI;
		case LAG_TEXTURE_COMPONENTS_RGB:
			return data.normalized ? -1 : GL_RGB32UI;
		case LAG_TEXTURE_COMPONENTS_RG:
			return data.normalized ? -1 : GL_RG32UI;
		case LAG_TEXTURE_COMPONENTS_R:
			return data.normalized ? -1 : GL_R32UI;
		default:
			return -1;
		}

	default:
		return -1;
	}
}