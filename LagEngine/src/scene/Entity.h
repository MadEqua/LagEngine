#pragma once

#include <vector>

#include "Types.h"
#include "SceneObject.h"
#include "IRenderable.h"
#include "Handle.h"

namespace Lag {
    class SubEntity;
    class Material;
    class Mesh;

    struct RenderOperation;
    enum class RenderMode : uint8;

    /*
    * Object represented by a Mesh (divided on SubMeshes) and a Material.
    * Each SubMesh of the received Mesh will be associated with a child SubEntity.
    * This implies that there's always at least one SubEntity.
    * Attach to a SceneNode to add to the scene.
    */
    class Entity : public SceneObject, public IRenderable {
    public:

        //defaultMaterial will be used if the meshes contain no material information
        Entity(uint32 name, Handle<Material> defaultMaterial, Handle<Mesh> mesh);
        ~Entity() override;

        void addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) override;
        void render(Renderer &renderer, RenderOperation &renderOperation) override;

    private:
        std::vector<SubEntity *> subEntities;

        Handle<Material> defaultMaterial;
        Handle<Mesh> mesh;
    };
}