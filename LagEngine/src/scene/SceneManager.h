#pragma once

#include "Scene.h"

#include <unordered_map>
#include <string>

#include "../renderer/IFrameListener.h"
#include "../io/IKeyboardListener.h"
#include "../io/ICursorListener.h"

namespace Lag
{
	class SceneManager : public IFrameListener,
		public IKeyboardListener,
		public ICursorListener
	{
	public:
		SceneManager();
		~SceneManager();

		void registerObservers();

		//Fill a RenderQueue with objects in range of the Camera of the received Viewport
		void addRenderablesToQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) const;

		void addScene(const std::string &name, Scene &scene);
		void removeScene(const std::string &name);
		void setActiveScene(const std::string &name);

		void clear();

		Scene& getActiveScene() const;
		inline bool hasActiveScene() const { return activeSceneName.size() > 0; }

		//Callbacks
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
		std::string activeSceneName;
		std::unordered_map<std::string, Scene*> sceneMap;
	};
}