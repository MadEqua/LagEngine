#pragma once

#include <string>
#include <unordered_map>

#include "../Types.h"
#include "RenderQueue.h"

namespace Lag
{
	enum RenderMode
	{
		LAG_RENDER_MODE_TRIANGLES,
		LAG_RENDER_MODE_TRIANGLE_STRIP,
		LAG_RENDER_MODE_TRIANGLE_FAN,
		LAG_RENDER_MODE_LINES,
		LAG_RENDER_MODE_LINE_STRIP,
		LAG_RENDER_MODE_LINE_LOOP,
		LAG_RENDER_MODE_POINTS,
		LAG_RENDER_MODE_PATCHES
	};

	class RenderTarget;
	class IGraphicsAPI;
	class SceneManager;
	class GpuProgram;
	class GpuBuffer;
	class InputDescription;

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

		//Main entry point for rendering
		void renderAllRenderTargets();

		//Bind objects and settings
		inline void setRenderMode(RenderMode mode) { actualRenderMode = mode; }
		void bindVertexBuffer(GpuBuffer &vertexBuffer);
		void bindIndexBuffer(GpuBuffer &indexBuffer);
		void bindGpuProgram(GpuProgram &gpuProgram);
		void bindInputDescription(InputDescription &inputDescription);

		//High-level render calls
		void renderVertices(const VertexData &vertexData);
		void renderIndexed(const VertexData &vertexData, const IndexData &indexData, uint32 baseVertex = 0);

		/*void renderMultiVertices(const VertexData *vertexData[], uint32 drawCount);
		void renderMultiIndexed(const VertexData *vertexData[], const IndexData *indexData[], uint32 drawCount);

		void renderVerticesInstanced(const VertexData &vertexData, uint32 instanceCount);
		void renderIndexedInstanced(const VertexData &vertexData, const IndexData &indexData, uint32 instanceCount);*/

		void clearColorBuffer(float value[4]);
		void clearDepthBuffer(float value);
		void clearStencilBuffer(int32 value);
		void clearDepthAndStencilBuffer(float depth, int32 stencil);


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

		SceneManager &sceneManager;
		IGraphicsAPI &graphicsAPI;

		RenderQueue renderQueue;

		//TODO: uint32 batch, face and vx counter

		//Bound objects
		RenderMode actualRenderMode;
		GpuBuffer *boundVertexBuffer;
		GpuBuffer *boundIndexBuffer;
		GpuProgram *boundGpuProgram;
		InputDescription *boundInputDescription;
	};
}