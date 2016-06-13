#pragma once

#include <vector>
#include <string>

#include "../core/NamedContainer.h"
#include "../Types.h"
#include "SceneGraph.h"
#include <glm/vec3.hpp>

namespace Lag
{
	class PerspectiveCamera;
	class OrthographicCamera;
	class Camera;
	class Entity;
	class RenderQueue;
	class IRenderable;
	class Viewport;
	class Light;
	class PointLight;
	class DirectionalLight;
	class Color;
	class GpuProgram;
	class Sky;
	
	/*
	* Contains all structures that represent a Scene for different purposes (object hierarchy, culling, fast iteration, ...)
	* Can also create and manage Entities, Cameras, Lights, ...
	*/
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		Entity* createEntity(const std::string &meshName, const std::string &materialName);
		
		PerspectiveCamera& createPerspectiveCamera(float aspectRatio, float fovy, float nearPlane, float farPlane);
		OrthographicCamera& createOrthographicCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane);

		PointLight& createPointLight(const Color& color, const glm::vec3 &attenuation, bool castShadow = true);
		DirectionalLight& createDirectionalLight(const Color& color, const glm::vec3& direction, bool castShadow = true);

		//TODO: texture instead of material?
		void enableSky(const std::string &materialName);
		void disableSky();

		SceneObject* getSceneObject(uint32 name) const;

		//Fill a RenderQueue with objects in range of the Camera of the received Viewport
		void addRenderablesToQueue(RenderQueue &renderQueue, Viewport &viewport) const;

		//TODO: remove scene objects

		inline const std::vector<Entity*>& getEntities() const { return entityVector; }
		inline const std::vector<Camera*>& getCameras() const { return cameraVector; }
		inline const std::vector<PointLight*>& getPointLights() const { return pointLightVector; }
		inline const std::vector<DirectionalLight*>& getDirectionalLights() const { return directionalLightVector; }

		inline SceneGraph& getSceneGraph() { return sceneGraph; }

		inline uint8 getPointLightCount() const { return static_cast<uint8>(pointLightVector.size()); }
		inline uint8 getDirectionalLightCount() const { return static_cast<uint8>(directionalLightVector.size()); }

	private:
		SceneGraph sceneGraph;

		Sky *sky;

		//All SceneObjects organized by name. Main repository.
		NamedContainer<SceneObject> sceneObjectMap;

		//SceneObjects organized by specific types
		std::vector<Entity*> entityVector;
		std::vector<Camera*> cameraVector;
		std::vector<PointLight*> pointLightVector;
		std::vector<DirectionalLight*> directionalLightVector;

		//IRenderable SceneObjects
		std::vector<IRenderable*> renderableVector;

		//TODO spatial graph
	};
}