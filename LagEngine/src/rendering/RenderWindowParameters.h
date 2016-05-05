#pragma once

#include <string>
#include "../Types.h"

namespace Lag
{
	enum Platform
	{
		GLFW, ANDROID //TODO: GLFW is not really a platform :)
	};
	
	class RenderWindowParameters
	{
	public:
		uint32 width, height;
		std::string title;
		bool fullScreen;
		bool vsync;
		Platform platform; //TODO: really needed?
		uint32 maxFPS;

		//Hints
		uint32 msaaSamples;
		bool sRGB;

		RenderWindowParameters() :
			width(640), height(480),
			title("LagEngine Application"),
			fullScreen(false),
			vsync(true),
			platform(GLFW),
			maxFPS(60),

			msaaSamples(0),
			sRGB(true)
		{}
	};
}