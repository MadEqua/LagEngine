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
	
	class RenderTarget
	{
		GENERATE_OBSERVER_STORAGE(IRenderTargetListener)
		DECLARE_NOTIFY_METHOD(onPreRender, ARGS())
		DECLARE_NOTIFY_METHOD(onPostRender, ARGS())

	public:
		RenderTarget(uint32 width, uint32 height);
		virtual ~RenderTarget();

		virtual bool initialize() = 0;
		virtual void destroy() = 0;

		//Call to start the Rendering process.
		void startRender();
		
		//For double-buffered targets
		virtual void swapBuffers() {}

		Viewport& createViewport(const std::string &name, Camera &camera, float left = 0.0f, float top = 0.0f, float width = 1.0f, float height = 1.0f);

		Viewport* getViewport(const std::string &name);

		//TODO (fps, frametimes...)
		//FrameMetrics frameMetrics;

	protected:
		std::unordered_map<std::string, Viewport*> viewports;

		uint32 width, height;
	};
}