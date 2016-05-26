#pragma once

#include "../Types.h"
#include "IRenderTargetListener.h"

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
	class Viewport
	{
	public:
		//left, top, width and height are relative (0.0 to 1.0)
		Viewport(Camera &camera, RenderTarget &renderTarget, float left = 0.0f, float top = 0.0f, float width = 1.0f, float height = 1.0f);
		~Viewport();

		void addRenderablesToQueue(RenderQueue &renderQueue, SceneManager &sceneManager);

		inline const Camera& getCamera() { return camera;}

		uint32 getRealLeft() const;
		uint32 getRealTop() const;
		uint32 getRealWidth() const;
		uint32 getRealHeight() const;

	private:
		float left, top, width, height;

		Camera &camera;
		RenderTarget &renderTarget;

		void computeCameraAspectRatio() const;

		//Listening to resizes
		class RenderTargetListener : public IRenderTargetListener
		{
		public:
			RenderTargetListener(const Viewport &vp);
			
			virtual void onPreRender() override;
			virtual void onPostRender() override;
			virtual void onResize(int width, int height) override;

		private:
			const Viewport &viewport;
		};
		RenderTargetListener renderTargetListener;
	};
}