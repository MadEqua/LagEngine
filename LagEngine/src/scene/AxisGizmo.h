#pragma once

#include "IRenderable.h"
#include "VertexData.h"
#include "IndexData.h"
#include "SceneObject.h"
#include "Handle.h"
#include "Material.h"

namespace Lag {
    class AxisGizmo : public SceneObject, public IRenderable {
    public:
        AxisGizmo();

        void addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) override;
        void render(Renderer &renderer, RenderOperation &renderOperation) override;

    private:
        VertexData vertexData;
        IndexData indexData;
        Handle<Material> material;
    };
}
