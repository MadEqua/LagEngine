#pragma once

#include "Scene.h"

#include <unordered_map>
#include <string>

#include "IFrameListener.h"
#include "IKeyboardListener.h"
#include "ICursorListener.h"

namespace Lag {
    class SceneManager : public IFrameListener, public IKeyboardListener, public ICursorListener {
    public:
        SceneManager();
        ~SceneManager() override;

        void registerObservers();

        //Fill a RenderQueue with objects in range of the Camera of the received Viewport
        void addRenderablesToQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) const;

        void addScene(const std::string &name, Scene &scene);
        void removeScene(const std::string &name);
        void setActiveScene(const std::string &name);

        void clear();

        Scene &getActiveScene() const;

        inline bool hasActiveScene() const { return !activeSceneName.empty(); }

        //Callbacks
        void onFrameStart(float timePassed) override;
        void onFrameRenderingQueued(float timePassed) override;
        void onFrameEnd(float timePassed) override;

        void onKeyPress(int key, int modifier) override;
        void onKeyRelease(int key, int modifier) override;
        void onKeyRepeat(int key, int modifier) override;

        void onCursorMove(int x, int y) override;
        void onButtonPressed(int x, int y, int button, int modifiers) override;
        void onButtonReleased(int x, int y, int button, int modifiers) override;

    private:
        std::string activeSceneName;
        std::unordered_map<std::string, Scene *> sceneMap;
    };
}