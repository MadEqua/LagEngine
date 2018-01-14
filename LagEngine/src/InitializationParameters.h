#pragma once

#include <string>
#include "Types.h"

#include "renderer/Renderer.h"
#include "renderer/GraphicsApiType.h"

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
		std::string resourcesFolder;
		std::string imagesFolder;
		std::string materialsFolder;
		std::string meshesFolder;
		std::string shadersFolder;
		std::string scenesFolder;

		//Hints
		uint32 MSSAsamples;
		bool sRGB;

		GraphicsApiType graphicsApiType;

		InitializationParameters();
		explicit InitializationParameters(const std::string &iniFilePath);
	};
}