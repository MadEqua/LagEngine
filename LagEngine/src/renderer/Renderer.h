#pragma once

#include <string>

#include "../core/NamedContainer.h"
#include "../Types.h"
#include "RenderQueue.h"
#include "Color.h"
#include "IRenderTargetListener.h"
#include "../core/ObserverPattern.h"
#include "GpuProgramUniformFiller.h"
#include "../core/Timer.h"

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

	enum RenderPhase
	{
		LAG_RENDER_PHASE_DEPTH,
		LAG_RENDER_PHASE_COLOR
	};

	class RenderTarget;
	class RenderWindow;
	class RenderToTexture;
	class RenderWindow;
	class IGraphicsAPI;
	class SceneManager;
	class GpuProgram;
	class GpuBuffer;
	class InputDescription;
	class Viewport;
	class Texture;
	class IFrameListener;
	enum TextureType;

	/*
	* Encapsulating all the texture bindings to the various binding points.
	*/
	class TextureBindings
	{
	public:
		void setAsBound(const Texture &tex, uint8 unit);
		const Texture* getBinding(uint8 unit) const;

		std::unordered_map<uint8, const Texture*> bindings;
	};

	/*
	* Top level renderer. All the rendering process starts here culminating on concrete calls to a IGraphicsAPI.
	* 
	* TODO: describe the process...
	*/
	class Renderer
	{
		LAG_GENERATE_OBSERVER_STORAGE(IFrameListener)
		LAG_DECLARE_NOTIFY_METHOD(onFrameStart, LAG_ARGS(float timePassed))
		LAG_DECLARE_NOTIFY_METHOD(onFrameRenderingQueued, LAG_ARGS(float timePassed))
		LAG_DECLARE_NOTIFY_METHOD(onFrameEnd, LAG_ARGS(float timePassed))

	public:
		Renderer(IGraphicsAPI &graphicsAPI, SceneManager &sceneManager);
		~Renderer();

		static const uint8 MAX_POINT_LIGHTS = 8;
		static const uint8 MAX_DIRECTIONAL_LIGHTS = 4;

		//Entry points for rendering
		void startRenderingLoop(uint32 maxFps);
		void stopRenderingLoop();
		void renderOneFrame();

		//TODO: should render windows be created here instead of added? (problem: they are platform specific)
		uint16 addRenderWindow(RenderWindow &renderWindow);
		uint16 createRenderToTexture(uint32 width, uint32 height);
		void removeRenderToTexture(uint16 name);
		RenderToTexture* getRenderToTexture(uint16 name) const;

		//Bind objects and settings
		inline void setRenderMode(RenderMode mode) { actualRenderMode = mode; }
		void bindVertexBuffer(const GpuBuffer &vertexBuffer);
		void bindIndexBuffer(const GpuBuffer &indexBuffer);
		void bindGpuProgram(const GpuProgram &gpuProgram);
		void bindInputDescription(const InputDescription &inputDescription);
		void bindRenderTarget(const RenderTarget &renderTarget);
		void bindViewport(const Viewport &viewport);
		void bindTexture(const Texture &texture, uint8 unit = 0);


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

		void setDepthTestEnabled(bool enabled);
		void setDepthWritingEnabled(bool enabled);

		//TODO
		/** The RenderSystem will keep a count of tris rendered, this resets the count. */
		//void beginGeometryCount();
		/** Reports the number of tris rendered since the last _beginGeometryCount call. */
		//uint32 getFaceCount(void) const;
		/** Reports the number of batches rendered since the last _beginGeometryCount call. */
		//uint32 getBatchCount(void) const;
		/** Reports the number of vertices passed to the renderer since the last _beginGeometryCount call. */
		//uint32 getVertexCount(void) const;

		inline GpuProgramUniformFiller& getUniformFiller() { return uniformFiller; }

	private:
		NamedContainer<RenderTarget> *renderTargets;
		RenderWindow *renderWindow;

		bool shouldLoop;
		uint64 actualFrame;

		//Frame Timing
		Timer wholeFrameTimer;
		Timer frameStartTimer;
		Timer frameEndTimer;
		Timer frameQueuedTimer;
		
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
		const RenderTarget *boundRenderTarget;
		const Viewport *boundViewport;
		TextureBindings boundTextures;

		//This exists for the case where a frame finishes with the same GpuProgram than the next one will start
		//Force uniformFiller.onGpuProgramBind() to be called in that case, some uniforms may need to be updated.
		const GpuProgram* lastUsedGpuProgramOnFrame;

		//Listening to resizes. A resize may invalidate the current bound Viewport.
		class RenderWindowListener : public IRenderTargetListener
		{
		public:
			RenderWindowListener(Renderer &renderer) : renderer(renderer) {}

			virtual void onPreRender(RenderTarget &notifier) override {};
			virtual void onPostRender(RenderTarget &notifier) override {};
			virtual void onResize(RenderTarget &notifier, uint32 width, uint32 height) override;

		private:
			Renderer &renderer;
		};
		RenderWindowListener renderWindowListener;
	};
}