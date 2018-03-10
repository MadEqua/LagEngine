#pragma once

#include "../Types.h"
#include "IRenderTargetListener.h"
#include <string>

namespace Lag
{
	class Camera;
	class RenderTarget;
	class RenderQueue;
	class SceneManager;
	
	/*
	* Associates a Camera to a section of a RenderTarget.
	* Forces the camera to have a similar aspect ratio to that section of the RenderTarget.
	*/
	class Viewport : public IRenderTargetListener
	{
	public:
		//left, bottom, width and height are relative (0.0 to 1.0)
		Viewport(uint16 name, Camera &camera, RenderTarget &renderTarget, 
			float left = 0.0f, float bottom = 0.0f, float width = 1.0f, float height = 1.0f);

		~Viewport();

		void addRenderablesToQueue(RenderQueue &renderQueue, SceneManager &sceneManager, RenderTarget &renderTarget);

		inline const Camera& getCamera() const { return camera;}
		inline const uint16 getName() const { return name; }

		uint32 getRealLeft() const;
		uint32 getRealBottom() const;
		uint32 getRealWidth() const;
		uint32 getRealHeight() const;

	private:
		uint16 name;

		float left, bottom, width, height;

		Camera &camera;
		RenderTarget &renderTarget;

		void computeCameraAspectRatio() const;

		virtual void onPreRender(RenderTarget &notifier) override;
		virtual void onPostRender(RenderTarget &notifier) override;
		virtual void onResize(RenderTarget &notifier, uint32 width, uint32 height) override;
	};
}