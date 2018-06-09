#include "ImageManager.h"

using namespace Lag;

ImageManager::ImageManager(ImageBuilder *builder) :
        XmlResourceManager("ImageManager", builder) {
}

void ImageManager::initializeFallbackObject() {
    fallbackObject = get("defaultImage");
}

ImageBuilder::ImageBuilder(const XmlResourceBuilderData &xmlResourceData) :
        XmlResourceBuilder(xmlResourceData) {
}

Image *ImageBuilder::parseAndCreate(const std::string &path, const TiXmlElement &element) const {
    return new Image(path + '/' + parseFileAttribute(element), parseImageData(element));
}

ImageData ImageBuilder::parseImageData(const TiXmlElement &element) {
    const char *componentsStr = element.Attribute("components");
    const char *componentTypeStr = element.Attribute("componentType");
    const char *normalizedStr = element.Attribute("normalized");
    const char *sRgbStr = element.Attribute("sRGB");

    ImageData data;
    if (componentsStr != nullptr)
        data.components = parseComponents(componentsStr);
    if (componentTypeStr != nullptr)
        data.componentType = parseComponentType(componentTypeStr);
    if (normalizedStr != nullptr)
        data.normalized = parseBool(normalizedStr);
    if (sRgbStr != nullptr)
        data.sRGB = parseBool(sRgbStr);
    return data;
}

ImageComponents ImageBuilder::parseComponents(const std::string &c) {
    if (c == "R") return ImageComponents::R;
    else if (c == "RG") return ImageComponents::RG;
    else if (c == "RGB") return ImageComponents::RGB;
    else if (c == "RGBA") return ImageComponents::RGBA;
    else return ImageComponents::RGB;
}

ImageComponentType ImageBuilder::parseComponentType(const std::string &type) {
    if (type == "FLOAT16" || type == "HALF_FLOAT") return ImageComponentType::FLOAT16;
    else if (type == "FLOAT32" || type == "FLOAT") return ImageComponentType::FLOAT32;

    else if (type == "INT8") return ImageComponentType::INT8;
    else if (type == "INT16")return ImageComponentType::INT16;
    else if (type == "INT32") return ImageComponentType::INT32;

    else if (type == "UINT8") return ImageComponentType::UINT8;
    else if (type == "UINT16")return ImageComponentType::UINT16;
    else if (type == "UINT32") return ImageComponentType::UINT32;

    else return ImageComponentType::UINT8;
}

bool ImageBuilder::parseBool(const std::string &str) {
    return str == "true" || str == "TRUE" || str == "1";
}