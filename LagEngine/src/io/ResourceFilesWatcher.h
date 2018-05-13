#pragma once

#include <string>
#include "../core/Timer.h"
#include "../renderer/IFrameListener.h"

namespace filewatch
{
	template<class T> class FileWatch;
}

namespace Lag
{
	class InitializationParameters;

	class ResourceFilesWatcher : public IFrameListener
	{
	public:
		ResourceFilesWatcher();
		~ResourceFilesWatcher();
		void run(const InitializationParameters &initializationParameters);

		void handleResourceChange(const InitializationParameters &initializationParameters, const std::string &path);

		virtual void onFrameStart(float timePassed) override {}
		virtual void onFrameRenderingQueued(float timePassed) override {}
		virtual void onFrameEnd(float timePassed) override;

	private:
		filewatch::FileWatch<std::string> *fileWatcher;
		Timer timer;

		bool shouldReloadMaterials;
		bool shouldReloadTextures;
		bool shouldReloadShaders;
	};

}