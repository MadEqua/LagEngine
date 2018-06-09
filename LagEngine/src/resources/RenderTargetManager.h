#pragma once

#include "Manager.h"
#include "RenderTarget.h"
#include "Types.h"
#include "InitializationParameters.h"

namespace Lag {
    class RenderWindow;

    class RenderToTexture;

    class RenderTargetBuilder : public IManagedObjectBuilder<uint32, RenderTarget> {
    public:
        RenderTargetBuilder();

        void setBuildRenderWindow(const InitializationParameters &parameters);
        void setBuildRenderToTexture(uint32 width, uint32 height, RenderPhase renderPhase);

    protected:
        const InitializationParameters *initializationParameters;
        uint32 width, height;
        RenderPhase renderPhase;
    };


    /*
    * Creates and manages all RenderTargets (Windows and RenderTextures).
    * Only supporting one RenderWindow, it will always have the name 0.
    * Unlike the base Manager this contains two Builders.
    * The reason is: RenderWindows are built by the platform (GLFW, Win32 API, ...)
    * but RenderToTextures are built by the Graphics API (GL, Direct3D, ...).
    */
    class RenderTargetManager : public Manager<uint32, RenderTarget> {
    public:
        RenderTargetManager(RenderTargetBuilder *renderWindowBuilder, RenderTargetBuilder *renderTextureBuilder);
        ~RenderTargetManager() override;

        //Convenience methods for the most common operations
        Handle<RenderTarget> getRenderWindow(const InitializationParameters &parameters);
        RenderWindow *
        getRenderWindow(); //This is meant to return the *only* RenderWindow. For creating use the overload with arguments.

        Handle<RenderTarget> getRenderToTexture(uint32 width, uint32 height, RenderPhase renderPhase);

        /* BasicState stands for:
         * - Only RenderWindow, no other RenderTargets
         * - No Viewports on RenderWindow
         */
        void resetToBasicState();

        inline RenderTargetBuilder &getRenderTextureToBuilder() { return *renderTextureBuilder; }
        inline RenderTargetBuilder &getRenderWindowToBuilder() { return *renderWindowBuilder; }

    protected:
        uint32 nextName;
        RenderTargetBuilder *renderWindowBuilder, *renderTextureBuilder;

        uint32 getNextName();
    };
}