#pragma once

namespace Lag {
    class RenderQueue;
    class Renderer;
    class Viewport;
    struct RenderOperation;
    class RenderTarget;

    /*
    * A Renderable should be able to add itself to a RenderQueue and
    * know how to render itself by issuing calls to the Renderer.
    */
    class IRenderable {
    public:
        virtual ~IRenderable() = default;

        //The Viewport is passed to the Renderable and should be placed on the RenderOperation added to the queue
        virtual void addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) = 0;

        //Everything is bound and ready to issue render calls. Useful for setting shader uniforms, for example
        virtual void preRender(Renderer &renderer, const RenderOperation &renderOperation) = 0;

        //Called when its time for the Renderable to render itself.
        //The RenderOperation passed is the one added to the RenderQueue by this Renderable
        virtual void render(Renderer &renderer, const RenderOperation &renderOperation) = 0;
    };
}