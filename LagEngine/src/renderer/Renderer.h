#pragma once

#include <string>
#include <unordered_map>

#include "../Types.h"
#include "RenderQueue.h"

namespace Lag
{
	class RenderTarget;
	class IGraphicsAPI;
	class SceneManager;

	/*
	* Top level renderer. All the rendering process starts here culminating on concrete calls to a IGraphicsAPI.
	* 
	* TODO: describe the process...
	*/
	class Renderer
	{
	public:
		Renderer(IGraphicsAPI &graphicsAPI, SceneManager &sceneManager);
		~Renderer();

		//TODO: find a way for creating all the RenderTargets within the renderer.
		void addRenderTarget(const std::string &name, RenderTarget &renderTarget);
		void removeRenderTarget(const std::string &name);

		void renderAllRenderTargets();

		//TODO
		/** The RenderSystem will keep a count of tris rendered, this resets the count. */
		//void beginGeometryCount();
		/** Reports the number of tris rendered since the last _beginGeometryCount call. */
		//uint32 getFaceCount(void) const;
		/** Reports the number of batches rendered since the last _beginGeometryCount call. */
		//uint32 getBatchCount(void) const;
		/** Reports the number of vertices passed to the renderer since the last _beginGeometryCount call. */
		//uint32 getVertexCount(void) const;

	protected:
		std::unordered_map<std::string, RenderTarget*> renderTargets;

		//uint32 batch, face and vx counter

		SceneManager &sceneManager;
		IGraphicsAPI &graphicsAPI;

		RenderQueue renderQueue;
	};
}