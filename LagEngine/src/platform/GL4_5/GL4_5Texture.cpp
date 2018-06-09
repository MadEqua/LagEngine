#include "GL4_5Texture.h"

using namespace Lag;

GL4_5Texture::GL4_5Texture(const std::string &imageName, const TextureData &data) :
        Texture(imageName, data) {
}

GL4_5Texture::GL4_5Texture(const std::vector<std::string> &imageNames, const TextureData &data) :
        Texture(imageNames, data) {
}

GL4_5Texture::GL4_5Texture(const ImageData &imageData, const TextureData &textureData) :
        Texture(imageData, textureData) {
}

bool GL4_5Texture::loadImplementation() {
    glCreateTextures(getGLType(), 1, &handle);

    if (!Texture::loadImplementation())
        return false;

    initStorage();
    sendData();
    setParameters();

    return true;
}

void GL4_5Texture::unloadImplementation() {
    glDeleteTextures(1, &handle);
    Texture::unloadImplementation();
}

void GL4_5Texture::initStorage() const {
    switch (textureData.type) {
        case TextureType::TYPE_1D:
            glTextureStorage1D(handle, textureData.mipmaps, getPixelDataSizedFormatGL(), imageData.width);
            break;

        case TextureType::TYPE_2D:
            glTextureStorage2D(handle, textureData.mipmaps, getPixelDataSizedFormatGL(),
                               imageData.width, imageData.height);
            break;

        case TextureType::TYPE_3D:
            glBindTexture(GL_TEXTURE_CUBE_MAP, handle); //bind seems to be required for cube maps
            glTexStorage2D(GL_TEXTURE_CUBE_MAP, textureData.mipmaps, getPixelDataSizedFormatGL(),
                           imageData.width, imageData.height);
            break;
    }
}

void GL4_5Texture::sendData() const {
    if (!images.empty()) {
        const Image *image = images[0].get();

        switch (textureData.type) {
            case TextureType::TYPE_1D:
                glTextureSubImage1D(handle, 0, 0, imageData.width,
                                    getPixelDataFormatGL(), getPixelDataTypeGL(), image->getRawDataPointer());
                break;

            case TextureType::TYPE_2D:
                glTextureSubImage2D(handle, 0, 0, 0, imageData.width, imageData.height,
                                    getPixelDataFormatGL(), getPixelDataTypeGL(), image->getRawDataPointer());
                break;

            case TextureType::TYPE_3D:
                glBindTexture(GL_TEXTURE_CUBE_MAP, handle); //bind seems to be required for cube maps

                for (int i = 0; i < 6; ++i) {
                    const Image *face = images[i].get();
                    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                                    0, 0, 0,
                                    imageData.width, imageData.height,
                                    getPixelDataFormatGL(), getPixelDataTypeGL(),
                                    face->getRawDataPointer());
                }
                break;
        }

        if (textureData.mipmaps > 1)
            glGenerateTextureMipmap(handle);
    }
}

void GL4_5Texture::setParameters() const {
    glTextureParameteri(handle, GL_TEXTURE_WRAP_S, getWrappingModeGL(textureData.wrappingMode[0]));
    glTextureParameteri(handle, GL_TEXTURE_WRAP_T, getWrappingModeGL(textureData.wrappingMode[1]));
    glTextureParameteri(handle, GL_TEXTURE_WRAP_R, getWrappingModeGL(textureData.wrappingMode[2]));

    glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, getFilteringModeGL(textureData.minificationFilteringMode));
    glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, getFilteringModeGL(textureData.magnificationFilteringMode));
}

GLenum GL4_5Texture::getGLType() const {
    switch (textureData.type) {
        case TextureType::TYPE_1D:
            return GL_TEXTURE_1D;
        case TextureType::TYPE_2D:
            return GL_TEXTURE_2D;
        case TextureType::TYPE_3D:
            return GL_TEXTURE_CUBE_MAP;
        default:
            return 0;
    }
}

GLenum GL4_5Texture::getPixelDataFormatGL() const {
    switch (imageData.components) {
        case ImageComponents::RGBA:
            return GL_RGBA;
        case ImageComponents::RGB:
            return GL_RGB;
        case ImageComponents::RG:
            return GL_RG;
        case ImageComponents::R:
            return GL_RED;
        default:
            return 0;
    }
}

GLenum GL4_5Texture::getPixelDataTypeGL() const {
    switch (imageData.componentType) {
        case ImageComponentType::FLOAT32:
            return GL_FLOAT;

        case ImageComponentType::INT8:
            return GL_BYTE;
        case ImageComponentType::INT16:
            return GL_SHORT;
        case ImageComponentType::INT32:
            return GL_INT;

        case ImageComponentType::UINT8:
            return GL_UNSIGNED_BYTE;
        case ImageComponentType::UINT16:
            return GL_UNSIGNED_SHORT;
        case ImageComponentType::UINT32:
            return GL_UNSIGNED_INT;

        default:
            return 0;
    }
}

GLenum GL4_5Texture::getPixelDataSizedFormatGL() const {
    switch (textureData.dataType) {
        case TextureDataType::DEPTH:
            switch (imageData.componentType) {
                case ImageComponentType::FLOAT32:
                    return GL_DEPTH_COMPONENT32F;
                case ImageComponentType::UINT16:
                    return GL_DEPTH_COMPONENT16;
                case ImageComponentType::UINT24:
                    return GL_DEPTH_COMPONENT24;
                case ImageComponentType::UINT32:
                    return GL_DEPTH_COMPONENT32;
                default:
                    return 0;
            }

        case TextureDataType::DEPTH_STENCIL:
            switch (imageData.componentType) {
                case ImageComponentType::FLOAT32:
                    return GL_DEPTH32F_STENCIL8;
                case ImageComponentType::UINT24:
                    return GL_DEPTH24_STENCIL8;
                default:
                    return 0;
            }

        case TextureDataType::STENCIL:
            switch (imageData.componentType) {
                case ImageComponentType::UINT1:
                    return GL_STENCIL_INDEX1;
                case ImageComponentType::UINT4:
                    return GL_STENCIL_INDEX4;
                case ImageComponentType::UINT8:
                    return GL_STENCIL_INDEX8;
                case ImageComponentType::UINT16:
                    return GL_STENCIL_INDEX16;
                default:
                    return 0;
            }

        case TextureDataType::COLOR:
            switch (imageData.componentType) {
                case ImageComponentType::FLOAT16:
                    switch (imageData.components) {
                        case ImageComponents::RGBA:
                            return GL_RGBA16F;
                        case ImageComponents::RGB:
                            return GL_RGB16F;
                        case ImageComponents::RG:
                            return GL_RG16F;
                        case ImageComponents::R:
                            return GL_R16F;
                    }

                case ImageComponentType::FLOAT32:
                    switch (imageData.components) {
                        case ImageComponents::RGBA:
                            return GL_RGBA32F;
                        case ImageComponents::RGB:
                            return GL_RGB32F;
                        case ImageComponents::RG:
                            return GL_RG32F;
                        case ImageComponents::R:
                            return GL_R32F;
                    }

                case ImageComponentType::INT8:
                    switch (imageData.components) {
                        case ImageComponents::RGBA:
                            return imageData.normalized ? GL_RGBA8_SNORM : GL_RGBA8I;
                        case ImageComponents::RGB:
                            return imageData.normalized ? GL_RGB8_SNORM : GL_RGB8I;
                        case ImageComponents::RG:
                            return imageData.normalized ? GL_RG8_SNORM : GL_RG8I;
                        case ImageComponents::R:
                            return imageData.normalized ? GL_R8_SNORM : GL_R8I;
                    }

                case ImageComponentType::INT16:
                    switch (imageData.components) {
                        case ImageComponents::RGBA:
                            return imageData.normalized ? GL_RGBA16_SNORM : GL_RGBA16I;
                        case ImageComponents::RGB:
                            return imageData.normalized ? GL_RGB16_SNORM : GL_RGB16I;
                        case ImageComponents::RG:
                            return imageData.normalized ? GL_RG16_SNORM : GL_RG16I;
                        case ImageComponents::R:
                            return imageData.normalized ? GL_R16_SNORM : GL_R16I;
                    }

                case ImageComponentType::INT32:
                    switch (imageData.components) {
                        case ImageComponents::RGBA:
                            return GL_RGBA32I;
                        case ImageComponents::RGB:
                            return GL_RGB32I;
                        case ImageComponents::RG:
                            return GL_RG32I;
                        case ImageComponents::R:
                            return GL_R32I;
                    }

                case ImageComponentType::UINT8:
                    switch (imageData.components) {
                        case ImageComponents::RGBA:
                            return imageData.normalized ? (imageData.sRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8) : GL_RGBA8UI;
                        case ImageComponents::RGB:
                            return imageData.normalized ? (imageData.sRGB ? GL_SRGB8 : GL_RGB8) : GL_RGB8UI;
                        case ImageComponents::RG:
                            return imageData.normalized ? GL_RG8 : GL_RG8UI;
                        case ImageComponents::R:
                            return imageData.normalized ? GL_R8 : GL_R8UI;
                    }

                case ImageComponentType::UINT16:
                    switch (imageData.components) {
                        case ImageComponents::RGBA:
                            return imageData.normalized ? GL_RGBA16 : GL_RGBA16UI;
                        case ImageComponents::RGB:
                            return imageData.normalized ? GL_RGB16 : GL_RGB16UI;
                        case ImageComponents::RG:
                            return imageData.normalized ? GL_RG16 : GL_RG16UI;
                        case ImageComponents::R:
                            return imageData.normalized ? GL_R16 : GL_R16UI;
                    }

                case ImageComponentType::UINT32:
                    switch (imageData.components) {
                        case ImageComponents::RGBA:
                            return imageData.normalized ? -1 : GL_RGBA32UI;
                        case ImageComponents::RGB:
                            return imageData.normalized ? -1 : GL_RGB32UI;
                        case ImageComponents::RG:
                            return imageData.normalized ? -1 : GL_RG32UI;
                        case ImageComponents::R:
                            return imageData.normalized ? -1 : GL_R32UI;
                        default:
                            return 0;
                    }
            }
    }
    return 0;
}

GLint GL4_5Texture::getFilteringModeGL(TextureFilteringMode mode) const {
    switch (mode) {
        case TextureFilteringMode::NEAREST:
            return GL_NEAREST;
        case TextureFilteringMode::LINEAR:
            return GL_LINEAR;
        case TextureFilteringMode::NEAREST_MIPMAP_NEAREST:
            return GL_NEAREST_MIPMAP_NEAREST;
        case TextureFilteringMode::NEAREST_MIPMAP_LINEAR:
            return GL_NEAREST_MIPMAP_LINEAR;
        case TextureFilteringMode::LINEAR_MIPMAP_NEAREST:
            return GL_LINEAR_MIPMAP_NEAREST;
        case TextureFilteringMode::LINEAR_MIPMAP_LINEAR:
            return GL_LINEAR_MIPMAP_LINEAR;
        default:
            return 0;
    }
}

GLint GL4_5Texture::getWrappingModeGL(TexturewWrappingMode mode) const {
    switch (mode) {
        case TexturewWrappingMode::REPEAT:
            return GL_REPEAT;
        case TexturewWrappingMode::MIRRORED_REPEAT:
            return GL_MIRRORED_REPEAT;
        case TexturewWrappingMode::CLAMP_TO_EDGE:
            return GL_CLAMP_TO_EDGE;
        case TexturewWrappingMode::CLAMP_TO_BORDER:
            return GL_CLAMP_TO_BORDER;
        default:
            return 0;
    }
}
