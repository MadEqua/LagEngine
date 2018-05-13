#pragma once

#include "scene/Scene.h"

namespace Lag
{
	class FreeCamera;
}

class TestScene2 : public Lag::Scene
{
public:
	virtual void onStart() override;
	virtual void onEnd() override;
	virtual void onInitializeViewports(Lag::RenderWindow &renderWindow) override;

private:
	Lag::FreeCamera *camera;
};

