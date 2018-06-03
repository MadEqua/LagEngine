#pragma once

#include "IPlatformFactory.h"

namespace Lag {
    class GLFW_GL4_5Factory : public IPlatformFactory {
    public:
        IGraphicsAPI *getGraphicsAPI() const override;
        InputManager *getInputManager(const RenderWindow &renderWindow) const override;

        RenderTargetBuilder *getWindowRenderTargetBuilder() const override;
        RenderTargetBuilder *getTextureRenderTargetBuilder() const override;
        TextureBuilder *getTextureBuilder(const XmlResourceBuilderData &xmlResourceData) const override;
        GpuBufferBuilder *getGpuBufferBuilder() const override;
        GpuProgramBuilder *getGpuProgramBuilder() const override;
        GpuProgramStageBuilder *getGpuProgramStageBuilder(const XmlResourceBuilderData &xmlResourceData) const override;
        InputDescriptionBuilder *getInputDescriptionBuilder() const override;
    };
}