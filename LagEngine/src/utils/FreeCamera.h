#pragma once

#include "../scene/PerspectiveCamera.h"
#include "../io/IKeyboardListener.h"
#include "../io/ICursorListener.h"
#include "../renderer/IFrameListener.h"

namespace Lag
{
	class Scene;
	
	/*
	* Utility class for a classic free movement camera using WASD and mouse.
	*/
	class FreeCamera :
		public IKeyboardListener,
		public ICursorListener,
		public IFrameListener
	{
	public:
		FreeCamera(Scene &scene, float fovy, float nearPlane, float farPlane, float moveSpeed);
		~FreeCamera();

		inline Camera& getCamera() const { return *camera; }

		virtual void onKeyPress(int key, int modifier) override;
		virtual void onKeyRelease(int key, int modifier) override;
		virtual void onKeyRepeat(int key, int modifier) override;

		virtual void onCursorMove(int x, int y) override;
		virtual void onButtonPressed(int x, int y, int button, int modifiers) override;
		virtual void onButtonReleased(int x, int y, int button, int modifiers) override;

		virtual void onFrameStart(float timePassed) override;
		virtual void onFrameRenderingQueued(float timePassed) override;
		virtual void onFrameEnd(float timePassed) override;

	protected:
		float moveSpeed;
		bool keyVector[4]; //wasd
		int lastCursor[2]; //xy

		PerspectiveCamera *camera;
		SceneNode *cameraYawNode;
		SceneNode *cameraPitchNode;
		SceneNode *cameraTranslationNode;
	};
}