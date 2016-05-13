#pragma once

#include <string>
#include <unordered_map>

#include "../Types.h"
#include "RenderQueue.h"
#include "GpuInterfaceType.h"

namespace Lag
{
	class RenderWindow;
	class RenderWindowParams;
	class RenderTarget;
	class Renderable;
	class IGraphicsAPI;
	class Viewport;
	class GpuProgram;
	class SceneManager;

	/*
	* Top level renderer. All the rendering process starts here culminating on concrete calls to a IGraphicsAPI.
	* 
	* TODO: describe the process...
	*/
	class Renderer
	{
	public:
		Renderer(SceneManager &sceneManager);
		~Renderer();

		bool initialize(const GpuInterfaceType gpuInterface);
		void shutdown();

		//TODO: find a way for creating all the RenderTargets within the renderer.
		void addRenderTarget(const std::string &name, RenderTarget &renderTarget);
		void removeRenderTarget(const std::string &name);

		void renderAllRenderTargets();

		//void setViewport(const Viewport &vp);

		/*void setWorldMatrix(const glm::mat4 &mat);
		void setViewMatrix(const glm::mat4 &mat);
		void setProjectionMatrix(const glm::mat4 &mat);*/

		void clearFrameBuffer(/*buffers, color?*/);

		void render(const Renderable &renderable);
		
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
		//std::unordered_map<std::string, GpuProgram*> gpuPrograms;

		//texture manager?
		//TextureManager *textureManager;

		//active viewport
		//uint32 batch, face and vx counter

		SceneManager &sceneManager;

		RenderQueue renderQueue;

		IGraphicsAPI *graphicsAPI;
	};
}