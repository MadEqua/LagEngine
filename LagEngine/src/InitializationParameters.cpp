#include "InitializationParameters.h"

#include "io/ini/IniParser.h"
#include "io/ini/IniSettings.h"

using namespace Lag;

InitializationParameters::InitializationParameters() :
	width(640), height(480),
	title("Default LagEngine Application"),
	fullScreen(false),
	vsync(true),
	maxFPS(60),
	resourcesFile("resources.xml"),

	MSSAsamples(0),
	sRGB(true),
	gpuInterfaceType(OPENGL_4)
{
}

InitializationParameters::InitializationParameters(const std::string &iniFilePath) : InitializationParameters()
{
	IniParser parser;
	if (parser.parse(iniFilePath))
	{
		const IniSettings &iniSettings = parser.getParsedIniSettings();
		width = iniSettings.getFieldAsBasicType<int>("width", width);
		height = iniSettings.getFieldAsBasicType<int>("height", height);
		title = iniSettings.getFieldAsString("title", title);
		fullScreen = iniSettings.getFieldAsBasicType<bool>("fullScreen", fullScreen);
		vsync = iniSettings.getFieldAsBasicType<bool>("vsync", vsync);
		maxFPS = iniSettings.getFieldAsBasicType<int>("maxFPS", maxFPS);
		resourcesFile = iniSettings.getFieldAsString("resourcesFile", resourcesFile);
		
		MSSAsamples = iniSettings.getFieldAsBasicType<int>("MSSAsamples", MSSAsamples);
		sRGB = iniSettings.getFieldAsBasicType<bool>("sRGB", sRGB);

		std::string gpuInterfaceStr = iniSettings.getFieldAsString("gpuInterfaceType", "OPENGL4");
		if (gpuInterfaceStr == "OPENGL4") gpuInterfaceType = OPENGL_4;
	}
}

InitializationParameters::~InitializationParameters()
{
}