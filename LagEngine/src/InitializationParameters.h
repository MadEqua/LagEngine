#pragma once

#include <string>
#include "Types.h"

#include "platform/PlatformType.h"

namespace Lag
{
	class InitializationParameters
	{
	public:
		uint32 width, height;
		std::string title;
		bool fullScreen;
		bool vsync;
		uint32 maxFPS;

		std::string resourcesFile;
		std::string appResourcesFolder;
		std::string lagResourcesFolder;
		std::string imagesFolder;
		std::string materialsFolder;
		std::string meshesFolder;
		std::string shadersFolder;

		//Hints
		uint32 MSAAsamples;
		bool sRGB;

		PlatformType platformType;

		InitializationParameters();
		explicit InitializationParameters(const std::string &iniFilePath);

		std::string getImagesFolder(bool lagResources) const;
		std::string getMaterialsFolder(bool lagResources) const;
		std::string getMeshesFolder(bool lagResources) const;
		std::string getShadersFolder(bool lagResources) const;
	};
}