#pragma once

#include "Texture.h"
#include "GL/glew.h"

#include <string>

namespace Lag {
    class GL4_5Texture : public Texture {
    public:
        inline GLuint getHandle() const { return handle; }

        GLenum getGLType() const;

    private:
        friend class GL4_5TextureBuilder;

        GL4_5Texture(const std::string &imageName, const TextureData &data);
        GL4_5Texture(const std::vector<std::string> &imageNames, const TextureData &data);
        GL4_5Texture(const ImageData &imageData, const TextureData &textureData);

        GLuint handle;

        bool loadImplementation() override;
        void unloadImplementation() override;

        void initStorage() const;
        void sendData() const;
        void setParameters() const;

        GLenum getPixelDataFormatGL() const;
        GLenum getPixelDataTypeGL() const;
        GLenum getPixelDataSizedFormatGL() const;
        GLint getFilteringModeGL(TextureFilteringMode mode) const;
        GLint getWrappingModeGL(TexturewWrappingMode mode) const;
    };
}