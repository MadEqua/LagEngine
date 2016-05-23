#pragma once

#include <string>
#include <unordered_map>

#include "../core/ObserverPattern.h"
#include "IRenderTargetListener.h"
#include "../Types.h"

namespace Lag
{
	class Viewport;
	class Camera;
	class RenderQueue;
	class SceneManager;
	
	class RenderTarget
	{
		//TODO: is this needed?
		LAG_GENERATE_OBSERVER_STORAGE(IRenderTargetListener)
		LAG_DECLARE_NOTIFY_METHOD(onPreRender, LAG_ARGS())
		LAG_DECLARE_NOTIFY_METHOD(onPostRender, LAG_ARGS())

	public:
		RenderTarget(uint32 width, uint32 height);
		virtual ~RenderTarget();

		virtual bool initialize() = 0;
		virtual void destroy() = 0;

		void addRenderablesToQueue(RenderQueue &renderQueue, SceneManager &sceneManager);
		
		//For double-buffered targets
		virtual void swapBuffers() {}

		Viewport& createViewport(const std::string &name, Camera &camera, float left = 0.0f, float top = 0.0f, float width = 1.0f, float height = 1.0f);

		Viewport* getViewport(const std::string &name) const;

		//TODO (fps, frametimes...)
		//FrameMetrics frameMetrics;

	protected:
		std::unordered_map<std::string, Viewport*> viewports;

		uint32 width, height;
	};
}