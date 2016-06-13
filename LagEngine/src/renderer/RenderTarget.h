#pragma once

#include <string>

#include "../core/NamedContainer.h"
#include "../core/ObserverPattern.h"
#include "../Types.h"

namespace Lag
{
	class Viewport;
	class Camera;
	class RenderQueue;
	class SceneManager;
	class IRenderTargetListener;
	
	class RenderTarget
	{
		LAG_GENERATE_OBSERVER_STORAGE(IRenderTargetListener)
		LAG_DECLARE_NOTIFY_METHOD(onPreRender, LAG_ARGS(RenderTarget &notifier))
		LAG_DECLARE_NOTIFY_METHOD(onPostRender, LAG_ARGS(RenderTarget &notifier))
		LAG_DECLARE_NOTIFY_METHOD(onResize, LAG_ARGS(RenderTarget &notifier, int width, int height))

	public:
		RenderTarget(uint32 width, uint32 height, bool isMainWindow = false);
		virtual ~RenderTarget();

		virtual bool initialize() = 0;
		virtual void destroy() = 0;

		virtual void resize(int width, int height);

		void addRenderablesToQueue(RenderQueue &renderQueue, SceneManager &sceneManager);
		
		//For double-buffered targets
		virtual void swapBuffers() {}

		Viewport& createViewport(Camera &camera, 
			float left = 0.0f, float top = 0.0f, float width = 1.0f, float height = 1.0f);

		Viewport* getViewport(uint16 name) const;

		inline uint32 getWidth() const { return width;}
		inline uint32 getHeight() const { return height; }
		inline bool isMainWindow() const { return mainWindow; }

		//TODO (fps, frametimes...)
		//FrameMetrics frameMetrics;

	protected:
		NamedContainer<Viewport> viewports;

		uint32 width, height;
		bool mainWindow;
	};
}