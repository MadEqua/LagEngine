#include "GL4Texture.h"
#include "../../resources/Image.h"

using namespace Lag;

GL4Texture::GL4Texture(const std::string &imageName, const TextureData &data) :
	Texture(imageName, data)
{
}

GL4Texture::GL4Texture(const std::vector<std::string> &imageNames, const TextureData &data) :
	Texture(imageNames, data)
{
}

GL4Texture::GL4Texture(const ImageData &imageData, const TextureData &textureData) :
	Texture(imageData, textureData)
{
}

bool GL4Texture::loadImplementation()
{
	GL_ERROR_PRINT(glCreateTextures(getGLType(), 1, &handle))

	if (!Texture::loadImplementation())
		return false;

	initStorage();
	sendData();
	setParameters();

	return true;
}

void GL4Texture::unloadImplementation()
{
	GL_ERROR_PRINT(glDeleteTextures(1, &handle))
}

void GL4Texture::initStorage() const
{
	switch (textureData.type)
	{
	case LAG_TEXTURE_TYPE_1D:
		GL_ERROR_PRINT(glTextureStorage1D(handle, textureData.mipmaps, getPixelDataSizedFormatGL(), imageData.width))
		break;

	case LAG_TEXTURE_TYPE_2D:
		GL_ERROR_PRINT(glTextureStorage2D(handle, textureData.mipmaps, getPixelDataSizedFormatGL(), 
			imageData.width, imageData.height))
		break;

	case LAG_TEXTURE_TYPE_CUBE:
		GL_ERROR_PRINT(glBindTexture(GL_TEXTURE_CUBE_MAP, handle)) //bind seems to be required for cube maps
		GL_ERROR_PRINT(glTexStorage2D(GL_TEXTURE_CUBE_MAP, textureData.mipmaps, getPixelDataSizedFormatGL(),
				imageData.width, imageData.height));
		break;
	default:
		break;
	}
}

void GL4Texture::sendData() const
{
	if (!images.empty())
	{
		const Image *image = images[0].get();

		switch (textureData.type)
		{
		case LAG_TEXTURE_TYPE_1D:
			GL_ERROR_PRINT(glTextureSubImage1D(handle, 0, 0, imageData.width,
				getPixelDataFormatGL(), getPixelDataTypeGL(), image->getRawDataPointer()))
				break;

		case LAG_TEXTURE_TYPE_2D:
			GL_ERROR_PRINT(glTextureSubImage2D(handle, 0, 0, 0, imageData.width, imageData.height,
				getPixelDataFormatGL(), getPixelDataTypeGL(), image->getRawDataPointer()))
				break;

		case LAG_TEXTURE_TYPE_CUBE:
			GL_ERROR_PRINT(glBindTexture(GL_TEXTURE_CUBE_MAP, handle)) //bind seems to be required for cube maps

				for (int i = 0; i < 6; ++i)
				{
					const Image *image = images[i].get();
					GL_ERROR_PRINT(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						0, 0, 0,
						imageData.width, imageData.height,
						getPixelDataFormatGL(), getPixelDataTypeGL(),
						image->getRawDataPointer()));
				}
				break;
		default:
			break;
		}

		if (textureData.mipmaps > 1)
			GL_ERROR_PRINT(glGenerateTextureMipmap(handle))
	}
}

void GL4Texture::setParameters() const
{

	GL_ERROR_PRINT(glTextureParameteri(handle, GL_TEXTURE_WRAP_S, getWrappingModeGL(textureData.wrappingMode[0])))
	GL_ERROR_PRINT(glTextureParameteri(handle, GL_TEXTURE_WRAP_T, getWrappingModeGL(textureData.wrappingMode[1])))
	GL_ERROR_PRINT(glTextureParameteri(handle, GL_TEXTURE_WRAP_R, getWrappingModeGL(textureData.wrappingMode[2])))

	GL_ERROR_PRINT(glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, getFilteringModeGL(textureData.minificationFilteringMode)))
	GL_ERROR_PRINT(glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, getFilteringModeGL(textureData.magnificationFilteringMode)))
}

GLenum GL4Texture::getGLType() const
{
	switch (textureData.type)
	{
	case LAG_TEXTURE_TYPE_1D:
		return GL_TEXTURE_1D;
	case LAG_TEXTURE_TYPE_2D:
		return GL_TEXTURE_2D;
	case LAG_TEXTURE_TYPE_CUBE:
		return GL_TEXTURE_CUBE_MAP;
	default:
		return GL_TEXTURE_2D;
	}
}

GLenum GL4Texture::getPixelDataFormatGL() const
{
	switch (imageData.components)
	{
	case LAG_IMAGE_COMPONENTS_RGBA:
		return GL_RGBA;
	case LAG_IMAGE_COMPONENTS_RGB:
		return GL_RGB;
	case LAG_IMAGE_COMPONENTS_RG:
		return GL_RG;
	case LAG_IMAGE_COMPONENTS_R:
		return GL_RED;
	default:
		return -1;
	}
}

GLenum GL4Texture::getPixelDataTypeGL() const
{
	switch (imageData.componentType)
	{
	case LAG_IMAGE_COMPONENT_TYPE_FLOAT32:
		return GL_FLOAT;

	case LAG_IMAGE_COMPONENT_TYPE_INT8:
		return GL_BYTE;
	case LAG_IMAGE_COMPONENT_TYPE_INT16:
		return GL_SHORT;
	case LAG_IMAGE_COMPONENT_TYPE_INT32:
		return GL_INT;

	case LAG_IMAGE_COMPONENT_TYPE_UINT8:
		return GL_UNSIGNED_BYTE;
	case LAG_IMAGE_COMPONENT_TYPE_UINT16:
		return GL_UNSIGNED_SHORT;
	case LAG_IMAGE_COMPONENT_TYPE_UINT32:
		return GL_UNSIGNED_INT;

	default:
		return -1;
	}
}

GLenum GL4Texture::getPixelDataSizedFormatGL() const
{
	switch (textureData.dataType)
	{
	case LAG_TEXTURE_DATA_TYPE_DEPTH:
		switch (imageData.componentType)
		{
		case LAG_IMAGE_COMPONENT_TYPE_FLOAT32:
			return GL_DEPTH_COMPONENT32F;
		case LAG_IMAGE_COMPONENT_TYPE_UINT16:
			return GL_DEPTH_COMPONENT16;
		case LAG_IMAGE_COMPONENT_TYPE_UINT24:
			return GL_DEPTH_COMPONENT24;
		case LAG_IMAGE_COMPONENT_TYPE_UINT32:
			return GL_DEPTH_COMPONENT32;
		default:
			return -1;
		}

	case LAG_TEXTURE_DATA_TYPE_DEPTH_STENCIL:
		switch (imageData.componentType)
		{
		case LAG_IMAGE_COMPONENT_TYPE_FLOAT32:
			return GL_DEPTH32F_STENCIL8;
		case LAG_IMAGE_COMPONENT_TYPE_UINT24:
			return GL_DEPTH24_STENCIL8;
		default:
			return -1;
		}

	case LAG_TEXTURE_DATA_TYPE_STENCIL:
		switch (imageData.componentType)
		{
		case LAG_IMAGE_COMPONENT_TYPE_UINT1:
			return GL_STENCIL_INDEX1;
		case LAG_IMAGE_COMPONENT_TYPE_UINT4:
			return GL_STENCIL_INDEX4;
		case LAG_IMAGE_COMPONENT_TYPE_UINT8:
			return GL_STENCIL_INDEX8;
		case LAG_IMAGE_COMPONENT_TYPE_UINT16:
			return GL_STENCIL_INDEX16;
		default:
			return -1;
		}

	case LAG_TEXTURE_DATA_TYPE_COLOR:
		switch (imageData.componentType)
		{
		case LAG_IMAGE_COMPONENT_TYPE_FLOAT16:
			switch (imageData.components)
			{
			case LAG_IMAGE_COMPONENTS_RGBA:
				return GL_RGBA16F;
			case LAG_IMAGE_COMPONENTS_RGB:
				return GL_RGB16F;
			case LAG_IMAGE_COMPONENTS_RG:
				return GL_RG16F;
			case LAG_IMAGE_COMPONENTS_R:
				return GL_R16F;
			default:
				return -1;
			}

		case LAG_IMAGE_COMPONENT_TYPE_FLOAT32:
			switch (imageData.components)
			{
			case LAG_IMAGE_COMPONENTS_RGBA:
				return GL_RGBA32F;
			case LAG_IMAGE_COMPONENTS_RGB:
				return GL_RGB32F;
			case LAG_IMAGE_COMPONENTS_RG:
				return GL_RG32F;
			case LAG_IMAGE_COMPONENTS_R:
				return GL_R32F;
			default:
				return -1;
			}

		case LAG_IMAGE_COMPONENT_TYPE_INT8:
			switch (imageData.components)
			{
			case LAG_IMAGE_COMPONENTS_RGBA:
				return imageData.normalized ? GL_RGBA8_SNORM : GL_RGBA8I;
			case LAG_IMAGE_COMPONENTS_RGB:
				return imageData.normalized ? GL_RGB8_SNORM : GL_RGB8I;
			case LAG_IMAGE_COMPONENTS_RG:
				return imageData.normalized ? GL_RG8_SNORM : GL_RG8I;
			case LAG_IMAGE_COMPONENTS_R:
				return imageData.normalized ? GL_R8_SNORM : GL_R8I;
			default:
				return -1;
			}

		case LAG_IMAGE_COMPONENT_TYPE_INT16:
			switch (imageData.components)
			{
			case LAG_IMAGE_COMPONENTS_RGBA:
				return imageData.normalized ? GL_RGBA16_SNORM : GL_RGBA16I;
			case LAG_IMAGE_COMPONENTS_RGB:
				return imageData.normalized ? GL_RGB16_SNORM : GL_RGB16I;
			case LAG_IMAGE_COMPONENTS_RG:
				return imageData.normalized ? GL_RG16_SNORM : GL_RG16I;
			case LAG_IMAGE_COMPONENTS_R:
				return imageData.normalized ? GL_R16_SNORM : GL_R16I;
			default:
				return -1;
			}

		case LAG_IMAGE_COMPONENT_TYPE_INT32:
			switch (imageData.components)
			{
			case LAG_IMAGE_COMPONENTS_RGBA:
				return GL_RGBA32I;
			case LAG_IMAGE_COMPONENTS_RGB:
				return GL_RGB32I;
			case LAG_IMAGE_COMPONENTS_RG:
				return GL_RG32I;
			case LAG_IMAGE_COMPONENTS_R:
				return GL_R32I;
			default:
				return -1;
			}

		case LAG_IMAGE_COMPONENT_TYPE_UINT8:
			switch (imageData.components)
			{
			case LAG_IMAGE_COMPONENTS_RGBA:
				return imageData.normalized ? (imageData.sRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8) : GL_RGBA8UI;
			case LAG_IMAGE_COMPONENTS_RGB:
				return imageData.normalized ? (imageData.sRGB ? GL_SRGB8 : GL_RGB8) : GL_RGB8UI;
			case LAG_IMAGE_COMPONENTS_RG:
				return imageData.normalized ? GL_RG8 : GL_RG8UI;
			case LAG_IMAGE_COMPONENTS_R:
				return imageData.normalized ? GL_R8 : GL_R8UI;
			default:
				return -1;
			}

		case LAG_IMAGE_COMPONENT_TYPE_UINT16:
			switch (imageData.components)
			{
			case LAG_IMAGE_COMPONENTS_RGBA:
				return imageData.normalized ? GL_RGBA16 : GL_RGBA16UI;
			case LAG_IMAGE_COMPONENTS_RGB:
				return imageData.normalized ? GL_RGB16 : GL_RGB16UI;
			case LAG_IMAGE_COMPONENTS_RG:
				return imageData.normalized ? GL_RG16 : GL_RG16UI;
			case LAG_IMAGE_COMPONENTS_R:
				return imageData.normalized ? GL_R16 : GL_R16UI;
			default:
				return -1;
			}

		case LAG_IMAGE_COMPONENT_TYPE_UINT32:
			switch (imageData.components)
			{
			case LAG_IMAGE_COMPONENTS_RGBA:
				return imageData.normalized ? -1 : GL_RGBA32UI;
			case LAG_IMAGE_COMPONENTS_RGB:
				return imageData.normalized ? -1 : GL_RGB32UI;
			case LAG_IMAGE_COMPONENTS_RG:
				return imageData.normalized ? -1 : GL_RG32UI;
			case LAG_IMAGE_COMPONENTS_R:
				return imageData.normalized ? -1 : GL_R32UI;
			default:
				return -1;
			}

		default:
			return -1;
		}

	default:
		return -1;
	}
}

GLint GL4Texture::getFilteringModeGL(TextureFilteringMode mode) const
{
	switch (mode)
	{
	case LAG_TEXTURE_FILTERING_MODE_NEAREST:
		return GL_NEAREST;
	case LAG_TEXTURE_FILTERING_MODE_LINEAR:
		return GL_LINEAR;
	case LAG_TEXTURE_FILTERING_MODE_NEAREST_MIPMAP_NEAREST:
		return GL_NEAREST_MIPMAP_NEAREST;
	case LAG_TEXTURE_FILTERING_MODE_NEAREST_MIPMAP_LINEAR:
		return GL_NEAREST_MIPMAP_LINEAR;
	case LAG_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_NEAREST:
		return GL_LINEAR_MIPMAP_NEAREST;
	case LAG_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_LINEAR:
		return GL_LINEAR_MIPMAP_LINEAR;
	default:
		return GL_NEAREST;
	}
}

GLint GL4Texture::getWrappingModeGL(TexturewWrappingMode mode) const
{
	switch (mode)
	{
	case LAG_TEXTURE_WRAPPING_MODE_REPEAT:
		return GL_REPEAT;
	case LAG_TEXTURE_WRAPPING_MODE_MIRRORED_REPEAT:
		return GL_MIRRORED_REPEAT;
	case LAG_TEXTURE_WRAPPING_MODE_CLAMP_TO_EDGE:
		return GL_CLAMP_TO_EDGE;
	case LAG_TEXTURE_WRAPPING_MODE_CLAMP_TO_BORDER:
		return GL_CLAMP_TO_BORDER;
	default:
		return GL_REPEAT;
	}
}
