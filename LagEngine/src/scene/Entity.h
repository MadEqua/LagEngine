#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Types.h"
#include "SceneObject.h"
#include "IRenderable.h"
#include "Handle.h"
#include "SubEntity.h"
#include "Mesh.h"
#include "ICollisionListener.h"

namespace Lag {
    class Material;
    class IBoundingVolume;

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
        void preRender(Renderer &renderer, const RenderOperation &renderOperation) override;
        void render(Renderer &renderer, const RenderOperation &renderOperation) override;

        //To be called by SubEntities
        virtual void onSubEntityPreRender(SubEntity &subEntity, Renderer &renderer, const RenderOperation &renderOperation);
        virtual void onSubEntityRender(SubEntity &subEntity, Renderer &renderer, const RenderOperation &renderOperation);

        void setMaterial(const std::string &materialName);
        void setMaterial(Handle<Material> material);
        void setMesh(const std::string &meshName);
        void setMesh(Handle<Mesh> mesh);

        void updateWorldSpaceBoundingVolume();
        inline const IBoundingVolume& getWorldSpaceBoundingVolume() { return *worldSpaceBoundingVolume; }

        inline const std::string& getColliderName() const { return colliderName; }
        inline bool isCollider() const { return collider; }
        void setNonCollider();
        void setAsCollider(const std::string &colliderName);

        virtual void onCollision(Entity &other);

        //Callbacks coming from Scene
        virtual void onFrameStart(float timePassed);
        virtual void onFrameRenderingQueued(float timePassed);
        virtual void onFrameEnd(float timePassed);

        virtual void onKeyPress(int key, int modifier);
        virtual void onKeyRelease(int key, int modifier);
        virtual void onKeyRepeat(int key, int modifier);

        virtual void onCursorMove(int x, int y);
        virtual void onButtonPressed(int x, int y, int button, int modifiers);
        virtual void onButtonReleased(int x, int y, int button, int modifiers);

    protected:
        std::vector<std::unique_ptr<SubEntity>> subEntities;

        Handle<Material> material;
        Handle<Mesh> mesh;

        bool collider;
        std::string colliderName;
        std::unique_ptr<IBoundingVolume> worldSpaceBoundingVolume;

#ifdef ENABLE_BV_GIZMOS
        Handle<Mesh> boundingVolumeMesh;
        Handle<Material> boundingVolumeMaterial;
#endif 
    };
}