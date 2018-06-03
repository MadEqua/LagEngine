#pragma once

#include <vector>
#include <string>

#include "NamedContainer.h"
#include "Types.h"
#include "SceneGraph.h"
#include "SceneObject.h"
#include "glm/vec3.hpp"

namespace Lag {

    class PerspectiveCamera;
    class OrthographicCamera;
    class Camera;
    class Entity;
    class RenderQueue;
    class IRenderable;
    class Viewport;
    class PointLight;
    class DirectionalLight;
    class Color;
    class Sky;
    class RenderTarget;
    class RenderWindow;

    /*
    * Set of Entities, Cameras, Lights and all that represnt an unique "state" of the application.
    * Applications should inherit and add to the SceneManager at least one Scene.
    * I/O and frame observers are already implemented, ready to be overriden as needed.
    *
    * Contains all structures that represent a Scene for different purposes (object hierarchy, culling, fast iteration, ...)
    * Can also create and manage Entities, Cameras, Lights, ...
    *
    * TODO spatial graph for culling
    */
    class Scene {
    public:
        virtual ~Scene() = default;

        void start();
        void end();
        void initializeViewports();

        Entity *createEntity(const std::string &meshName, const std::string &materialName);

        PerspectiveCamera &createPerspectiveCamera(float aspectRatio, float fovy, float nearPlane, float farPlane);
        OrthographicCamera &createOrthographicCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane);

        PointLight &createPointLight(const Color &color, const glm::vec3 &attenuation, bool castShadow = true);
        DirectionalLight &createDirectionalLight(const Color &color, const glm::vec3 &direction, bool castShadow = true);

        //TODO: texture instead of material?
        void enableSky(const std::string &materialName);
        void disableSky();

        SceneObject *getSceneObject(uint32 name) const;

        //Fill a RenderQueue with objects in range of the Camera of the received Viewport
        void addRenderablesToQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) const;

        //TODO: remove scene objects

        inline const std::vector<Entity *> &getEntities() const { return entityVector; }
        inline const std::vector<Camera *> &getCameras() const { return cameraVector; }
        inline const std::vector<PointLight *> &getPointLights() const { return pointLightVector; }
        inline const std::vector<DirectionalLight *> &getDirectionalLights() const { return directionalLightVector; }
        inline SceneGraph &getSceneGraph() { return sceneGraph; }
        inline uint8 getPointLightCount() const { return static_cast<uint8>(pointLightVector.size()); }
        inline uint8 getDirectionalLightCount() const { return static_cast<uint8>(directionalLightVector.size()); }

        //Callbacks coming from SceneManager
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
        friend class SceneManager;

        Scene();

        //Create all the scene hierarchy here
        virtual void onStart() = 0;

        //Destroy and deallocate any object created at onStart()
        virtual void onEnd() = 0;

        //Create and add the desired Viewports here
        virtual void onInitializeViewports(RenderWindow &renderWindow) = 0;

        void scheduleSceneChange(const std::string &name);

        SceneGraph sceneGraph;
        Sky *sky;

        bool shouldChangeScene;
        std::string sceneToChange;

        //All SceneObjects organized by name. Main repository.
        NamedContainer<SceneObject> sceneObjectMap;

        //SceneObjects organized by specific types
        std::vector<Entity *> entityVector;
        std::vector<Camera *> cameraVector;
        std::vector<PointLight *> pointLightVector;
        std::vector<DirectionalLight *> directionalLightVector;

        //IRenderable SceneObjects
        std::vector<IRenderable *> renderableVector;
    };
}