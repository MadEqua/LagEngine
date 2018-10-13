#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Types.h"
#include "SceneObject.h"
#include "IRenderable.h"
#include "Handle.h"
#include "SubEntity.h"

namespace Lag {
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
        Entity(const std::string &meshName, const std::string &materialName);
        Entity(Handle<Mesh> meshHandle, const std::string &materialName);
        Entity();

        void addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) override;
        void render(Renderer &renderer, RenderOperation &renderOperation) override;

        void setMaterial(const std::string &materialName);
        void setMaterial(Handle<Material> material);
        void setMesh(const std::string &meshName);
        void setMesh(Handle<Mesh> mesh);

    protected:
        std::vector<std::unique_ptr<SubEntity>> subEntities;

    private:
        Handle<Material> material;
        Handle<Mesh> mesh;

#ifdef ENABLE_AABB_GIZMOS
        Handle<Mesh> aabbMesh;
        Handle<Material> aabbMaterial;
#endif 
    };
}