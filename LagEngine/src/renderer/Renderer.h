#pragma once

#include <string>
#include <unordered_map>

#include "../Types.h"
#include "RenderQueue.h"
#include "Color.h"
#include "IRenderTargetListener.h"
#include "GpuProgramUniformFiller.h"

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
	class Viewport;
	class Texture;

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
		void bindVertexBuffer(const GpuBuffer &vertexBuffer);
		void bindIndexBuffer(const GpuBuffer &indexBuffer);
		void bindGpuProgram(const GpuProgram &gpuProgram);
		void bindInputDescription(const InputDescription &inputDescription);
		void bindViewport(const Viewport &viewport);
		void bindTexture(const Texture &texture);


		//High-level render calls
		void renderVertices(const VertexData &vertexData);
		void renderIndexed(const VertexData &vertexData, const IndexData &indexData, uint32 baseVertex = 0);

		/*void renderMultiVertices(const VertexData *vertexData[], uint32 drawCount);
		void renderMultiIndexed(const VertexData *vertexData[], const IndexData *indexData[], uint32 drawCount);

		void renderVerticesInstanced(const VertexData &vertexData, uint32 instanceCount);
		void renderIndexedInstanced(const VertexData &vertexData, const IndexData &indexData, uint32 instanceCount);*/

		void setClearColor(const Color &color);
		void setClearDepthValue(float value);
		void setClearStencilValue(int32 value);

		void clearColorBuffer();
		void clearDepthBuffer();
		void clearStencilBuffer();
		void clearDepthAndStencilBuffer();

		//TODO
		/** The RenderSystem will keep a count of tris rendered, this resets the count. */
		//void beginGeometryCount();
		/** Reports the number of tris rendered since the last _beginGeometryCount call. */
		//uint32 getFaceCount(void) const;
		/** Reports the number of batches rendered since the last _beginGeometryCount call. */
		//uint32 getBatchCount(void) const;
		/** Reports the number of vertices passed to the renderer since the last _beginGeometryCount call. */
		//uint32 getVertexCount(void) const;

		static const uint8 MAX_POINT_LIGHTS = 8;
		static const uint8 MAX_DIRECTIONAL_LIGHTS = 4;

	private:
		std::unordered_map<std::string, RenderTarget*> renderTargets;

		uint64 frameNumber;

		SceneManager &sceneManager;
		IGraphicsAPI &graphicsAPI;

		GpuProgramUniformFiller uniformFiller;
		RenderQueue renderQueue;

		Color clearColor;
		float depthClearValue;
		int32 stencilClearValue;

		//TODO: uint32 batch, face and vx counter

		//Bound objects
		RenderMode actualRenderMode;
		const GpuBuffer *boundVertexBuffer;
		const GpuBuffer *boundIndexBuffer;
		const GpuProgram *boundGpuProgram;
		const InputDescription *boundInputDescription;
		const Viewport *boundViewport;
		const Texture *boundTexture;

		//Listening to resizes. A resize may invalidate the current bound Viewport.
		class RenderTargetListener : public IRenderTargetListener
		{
		public:
			RenderTargetListener(Renderer &renderer) :
				renderer(renderer) {}

			virtual void onPreRender(RenderTarget &notifier) override {};
			virtual void onPostRender(RenderTarget &notifier) override {};
			virtual void onResize(RenderTarget &notifier, int width, int height) override;

		private:
			Renderer &renderer;
		};
		RenderTargetListener renderTargetListener;
	};
}