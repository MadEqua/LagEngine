#pragma once

namespace Lag
{
	class Camera;
	class RenderTarget;
	
	class Viewport
	{
	public:
		//left, top, width and height are relative (0.0 to 1.0)
		Viewport(Camera &camera, RenderTarget &renderTarget, float left = 0.0f, float top = 0.0f, float width = 1.0f, float height = 1.0f);
		~Viewport();

		void render();

	private:
		float left, top, width, height;

		Camera &camera;
		RenderTarget &renderTarget;
	};
}