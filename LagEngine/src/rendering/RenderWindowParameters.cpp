#include "RenderWindowParameters.h"

#include "../io/ini/IniManager.h"
#include "../io/ini/IniSettings.h"

using namespace Lag;

RenderWindowParameters::RenderWindowParameters() :
	width(640), height(480),
	title("Default LagEngine Application"),
	fullScreen(false),
	vsync(true),
	maxFPS(60),

	MSSAsamples(0),
	sRGB(true)
{
}

RenderWindowParameters::RenderWindowParameters(const std::string &iniFilePath) : RenderWindowParameters()
{
	IniSettings *iniSettings;
	if (IniManager::getInstance().parse(iniFilePath, &iniSettings))
	{
		width = iniSettings->getFieldAsBasicType<int>("width", width);
		height = iniSettings->getFieldAsBasicType<int>("height", height);
		title = iniSettings->getFieldAsString("title", title);
		fullScreen = iniSettings->getFieldAsBasicType<bool>("fullScreen", fullScreen);
		vsync = iniSettings->getFieldAsBasicType<bool>("vsync", vsync);
		maxFPS = iniSettings->getFieldAsBasicType<int>("maxFPS", maxFPS);
		MSSAsamples = iniSettings->getFieldAsBasicType<int>("MSSAsamples", MSSAsamples);
		sRGB = iniSettings->getFieldAsBasicType<bool>("sRGB", sRGB);
	}
}

RenderWindowParameters::~RenderWindowParameters()
{
}