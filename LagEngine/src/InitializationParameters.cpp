#include "InitializationParameters.h"

#include "IniParser.h"
#include "IniSettings.h"
#include "Constants.h"

using namespace Lag;

InitializationParameters::InitializationParameters() :
        width(640), height(480),
        title("Default LagEngine Application"),
        fullScreen(false),
        vsync(true),
        maxFPS(60),
        resourcesFile("resources.xml"),
        lagResourcesFolder("LagResources"),
        appResourcesFolder("resources"),
        imagesFolder("images"),
        materialsFolder("materials"),
        meshesFolder("meshes"),
        shadersFolder("shaders"),

        MSAAsamples(0),
        sRGB(true),
        platformType(PlatformType::GLFW_GL4_5) {
}

InitializationParameters::InitializationParameters(const std::string &iniFilePath) : InitializationParameters() {
    IniParser parser;
    if (parser.parse(iniFilePath)) {
        const IniSettings &iniSettings = parser.getParsedIniSettings();
        width = iniSettings.getFieldAsBasicType<uint32>("width", width);
        height = iniSettings.getFieldAsBasicType<uint32>("height", height);
        title = iniSettings.getFieldAsString("title", title);
        fullScreen = iniSettings.getFieldAsBasicType<bool>("fullScreen", fullScreen);
        vsync = iniSettings.getFieldAsBasicType<bool>("vsync", vsync);
        maxFPS = iniSettings.getFieldAsBasicType<uint32>("maxFPS", maxFPS);

        resourcesFile = iniSettings.getFieldAsString("resourcesFile", resourcesFile);
        appResourcesFolder = iniSettings.getFieldAsString("appResourcesFolder", appResourcesFolder);
        lagResourcesFolder = iniSettings.getFieldAsString("lagResourcesFolder", lagResourcesFolder);
        imagesFolder = iniSettings.getFieldAsString("imagesFolder", imagesFolder);
        materialsFolder = iniSettings.getFieldAsString("materialsFolder", materialsFolder);
        meshesFolder = iniSettings.getFieldAsString("meshesFolder", meshesFolder);
        shadersFolder = iniSettings.getFieldAsString("shadersFolder", shadersFolder);

        MSAAsamples = iniSettings.getFieldAsBasicType<uint32>("MSAAsamples", MSAAsamples);
        sRGB = iniSettings.getFieldAsBasicType<bool>("sRGB", sRGB);

        std::string platformTypeStr = iniSettings.getFieldAsString("platformType", GLFW_GL4_5);
        if (platformTypeStr == GLFW_GL4_5) platformType = PlatformType::GLFW_GL4_5;
    }
}

std::string InitializationParameters::getImagesFolder(bool lagResources) const {
    return (lagResources ? lagResourcesFolder : appResourcesFolder) + '/' + imagesFolder;
}

std::string InitializationParameters::getMaterialsFolder(bool lagResources) const {
    return (lagResources ? lagResourcesFolder : appResourcesFolder) + '/' + materialsFolder;
}

std::string InitializationParameters::getMeshesFolder(bool lagResources) const {
    return (lagResources ? lagResourcesFolder : appResourcesFolder) + '/' + meshesFolder;
}

std::string InitializationParameters::getShadersFolder(bool lagResources) const {
    return (lagResources ? lagResourcesFolder : appResourcesFolder) + '/' + shadersFolder;
}
