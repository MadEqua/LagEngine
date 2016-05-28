#pragma once

#include "IFrameListener.h"
#include "io/IKeyboardListener.h"
#include "io/ICursorListener.h"

namespace Lag 
{
	class RenderWindow;
	class SceneManager;
	class Root;
	class FreeCamera;
	class SceneNode;
}

class TestApplication : public Lag::IFrameListener, 
	public Lag::IKeyboardListener, 
	public Lag::ICursorListener
{
public:
	TestApplication();
	~TestApplication();

	bool start();

	virtual void onFrameStart(float timePassed) override;
	virtual void onFrameRenderingQueued(float timePassed) override;
	virtual void onFrameEnd(float timePassed) override;

	virtual void onKeyPress(int key, int modifier) override;
	virtual void onKeyRelease(int key, int modifier) override;
	virtual void onKeyRepeat(int key, int modifier) override;

	virtual void onCursorMove(int x, int y) override;
	virtual void onButtonPressed(int x, int y, int button, int modifiers) override;
	virtual void onButtonReleased(int x, int y, int button, int modifiers) override;

private:
	void createScene();
	void createSceneAux(Lag::SceneNode &center, float size, int count, int actualdepth, int maxdepth);

	Lag::RenderWindow *renderWindow;
	Lag::SceneManager *sceneManager;
	Lag::Root *root;
	Lag::FreeCamera *camera;
};

