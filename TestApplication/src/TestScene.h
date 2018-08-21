#pragma once

#include "Scene.h"

namespace Lag {
    class SceneNode;
    class FreeCamera;
}

class TestScene : public Lag::Scene {
public:
    TestScene(bool isScene1);

private:
    void onStart() override;
    void onEnd() override;
    void onInitializeViewports(Lag::RenderWindow &renderWindow) override;


    void onFrameRenderingQueued(float timePassed) override;
    void onKeyPress(int key, int modifier) override;

    void createSceneAux(Lag::SceneNode &center, float size, int count, int actualdepth, int maxdepth);
    void updateScene(float timePassed);

    bool isScene1;
    Lag::FreeCamera *camera;
    float time;
    int nodeName;
};

