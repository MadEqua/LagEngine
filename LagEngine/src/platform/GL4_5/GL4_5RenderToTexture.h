#pragma once

#include "RenderToTexture.h"
#include "Types.h"
#include "GL/glew.h"

namespace Lag {
    class GL4RenderToTexture : public RenderToTexture {
    public:
        GL4RenderToTexture(uint32 width, uint32 height, RenderPhase renderPhase = RenderPhase::COLOR);

        void attachColorTexture(const ImageData &imageData, const TextureData &textureData, uint8 layer = 0,
                                uint8 attachment = 0, uint8 mipmapLevel = 0) override;
        void attachDepthStencilTexture(const ImageData &imageData, const TextureData &textureData, uint8 layer = 0,
                                       uint8 mipmapLevel = 0) override;
        void attachDepthTexture(const ImageData &imageData, const TextureData &textureData, uint8 layer = 0,
                                uint8 mipmapLevel = 0) override;
        void attachStencilTexture(const ImageData &imageData, const TextureData &textureData, uint8 layer = 0,
                                  uint8 mipmapLevel = 0) override;

        inline GLuint getHandle() const { return handle; }

        bool loadImplementation() override;
        void unloadImplementation() override;

    private:
        GLuint handle;

        std::string generateTextureName(const char *type, uint8 layer, uint8 attachment);
        bool checkCompleteness() const override;
    };
}