#pragma once

#include "IRenderable.h"

namespace Lag {

    class Material;
    class SubMesh;
    class RenderQueue;
    class Entity;
    struct RenderOperation;

    class SubEntity : public IRenderable {
        friend class Entity;

    private:
        SubEntity(Entity &parent, Material &material, SubMesh &subMesh);
        explicit SubEntity(Entity &parent);

        void setMaterial(Material &material);
        void setSubMesh(SubMesh &mesh);

        void addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) override;
        void render(Renderer &renderer, RenderOperation &renderOperation) override;

        Material *material;
        SubMesh *subMesh;
        Entity &parent;
    };
}