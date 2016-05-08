#pragma once

#include <string>
#include "../Types.h"

#include "Renderer.h"

namespace Lag
{
	class RenderWindowParameters
	{
	public:
		uint32 width, height;
		std::string title;
		bool fullScreen;
		bool vsync;
		uint32 maxFPS;

		//Hints
		uint32 MSSAsamples;
		bool sRGB;

		GpuInterface gpuInterface;

		RenderWindowParameters();
		explicit RenderWindowParameters(const std::string &iniFilePath);
		~RenderWindowParameters();
	};
}