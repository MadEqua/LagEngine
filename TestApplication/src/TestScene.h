#pragma once

#include "scene/Scene.h"

namespace Lag
{
	class SceneNode;
	class FreeCamera;
}

class TestScene : public Lag::Scene
{
public:
	TestScene(bool isScene1);

private:
	virtual void onStartImplementation() override;
	virtual void onEndImplementation() override;

	virtual void onFrameRenderingQueued(float timePassed) override;
	virtual void onKeyPress(int key, int modifier) override;

	void createSceneAux(Lag::SceneNode &center, float size, int count, int actualdepth, int maxdepth);
	void updateScene(float timePassed);

	bool isScene1;
	Lag::FreeCamera *camera;
	float time;
};

