#pragma once

#include <string>

#include "renderer/IFrameListener.h"
#include "io/IKeyboardListener.h"
#include "io/ICursorListener.h"

namespace Lag 
{
	class RenderWindow;
	class Scene;
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
	void createScene(Lag::Scene *scene, bool isOne);
	void createSceneAux(Lag::Scene *scene, Lag::SceneNode &center, float size, int count, int actualdepth, int maxdepth);
	void updateScene(float timePassed);

	bool changeScene;
	std::string sceneToChange;

	float time;

	Lag::RenderWindow *renderWindow;
	Lag::Scene *scene1, *scene2;
	Lag::Root *root;
	Lag::FreeCamera *camera;
};

