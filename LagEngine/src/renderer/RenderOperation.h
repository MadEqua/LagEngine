#pragma once

#include "Types.h"

namespace Lag {
    class VertexData;
    class IndexData;
    class Material;
    class IRenderable;
    class Viewport;
    class RenderTarget;
    enum class RenderMode : uint8;

    /*
    * A RenderOperation represents a single render call to the graphics API.
    * Renderables provide this data to the RenderQueue,
    * which it uses for sorting and posterior render call dispatching.
    */
    struct RenderOperation {
        RenderTarget *renderTarget;
        VertexData *vertexData;
        IndexData *indexData;
        Material *material;
        IRenderable *renderable;
        Viewport *viewport;
        uint32 passId;
    };
}