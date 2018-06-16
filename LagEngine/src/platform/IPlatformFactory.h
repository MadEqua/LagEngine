#pragma once

#include <string>

namespace Lag {
    class IGraphicsAPI;
    class InputManager;
    class RenderTargetBuilder;
    class TextureBuilder;
    class GpuBufferBuilder;
    class GpuProgramBuilder;
    class InputDescriptionBuilder;
    class GpuProgramStageBuilder;
    class RenderWindow;
    struct XmlResourceBuilderData;


    /*
    * Factory to create all the platform specific Managers and classes.
    * By platform we mean the Windowing system and the Graphics API.
    */
    class IPlatformFactory {
    public:
        virtual ~IPlatformFactory() = default;

        virtual IGraphicsAPI *getGraphicsAPI() const = 0;
        virtual InputManager *getInputManager(const RenderWindow &renderWindow) const = 0;

        virtual RenderTargetBuilder *getWindowRenderTargetBuilder() const = 0;
        virtual RenderTargetBuilder *getTextureRenderTargetBuilder() const = 0;
        virtual TextureBuilder *getTextureBuilder(const XmlResourceBuilderData &xmlResourceData) const = 0;
        virtual GpuBufferBuilder *getGpuBufferBuilder() const = 0;
        virtual GpuProgramBuilder *getGpuProgramBuilder() const = 0;
        virtual GpuProgramStageBuilder *getGpuProgramStageBuilder(const XmlResourceBuilderData &xmlResourceData) const = 0;
        virtual InputDescriptionBuilder *getInputDescriptionBuilder() const = 0;
    };
}