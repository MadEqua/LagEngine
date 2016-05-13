#pragma once

#include <string>
#include "Types.h"

#include "renderer/Renderer.h"
#include "renderer/GpuInterfaceType.h"

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

		//Hints
		uint32 MSSAsamples;
		bool sRGB;

		GpuInterfaceType gpuInterfaceType;

		InitializationParameters();
		explicit InitializationParameters(const std::string &iniFilePath);
		~InitializationParameters();
	};
}