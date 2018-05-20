#include "ImageManager.h"

#include "tinyxml/tinyxml.h"
#include "Image.h"

using namespace Lag;

ImageManager::ImageManager(ImageBuilder* builder) :
	XmlResourceManager("ImageManager", builder)
{
}

void ImageManager::initializeFallbackObject()
{
	fallbackObject = get("defaultImage");
}

ImageBuilder::ImageBuilder(const XmlResourceBuilderData &xmlResourceData) :
	XmlResourceBuilder(xmlResourceData)
{
}

Image* ImageBuilder::parseAndCreate(const std::string &path, const TiXmlElement &element) const
{
	return new Image(path + '/' + parseFileAttribute(element), parseImageData(element));
}

ImageData ImageBuilder::parseImageData(const TiXmlElement &element)
{
	const char* componentsStr = element.Attribute("components");
	const char* componentTypeStr = element.Attribute("componentType");
	const char* normalizedStr = element.Attribute("normalized");
	const char* sRgbStr = element.Attribute("sRGB");

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

ImageComponents ImageBuilder::parseComponents(const std::string &c)
{
	if(c == "R") return LAG_IMAGE_COMPONENTS_R;
	else if (c == "RG") return LAG_IMAGE_COMPONENTS_RG;
	else if (c == "RGB") return LAG_IMAGE_COMPONENTS_RGB;
	else if (c == "RGBA") return LAG_IMAGE_COMPONENTS_RGBA;
	else return LAG_IMAGE_COMPONENTS_RGB;
}

ImageComponentType ImageBuilder::parseComponentType(const std::string &type)
{
	if(type == "FLOAT16" || type == "HALF_FLOAT") return LAG_IMAGE_COMPONENT_TYPE_FLOAT16;
	else if (type == "FLOAT32" || type == "FLOAT") return LAG_IMAGE_COMPONENT_TYPE_FLOAT32;
	
	else if (type == "INT8") return LAG_IMAGE_COMPONENT_TYPE_INT8;
	else if (type == "INT16")return LAG_IMAGE_COMPONENT_TYPE_INT16;
	else if (type == "INT32") return LAG_IMAGE_COMPONENT_TYPE_INT32;

	else if (type == "UINT8") return LAG_IMAGE_COMPONENT_TYPE_UINT8;
	else if (type == "UINT16")return LAG_IMAGE_COMPONENT_TYPE_UINT16;
	else if (type == "UINT32") return LAG_IMAGE_COMPONENT_TYPE_UINT32;

	else return LAG_IMAGE_COMPONENT_TYPE_UINT8;
}

bool ImageBuilder::parseBool(const std::string &str)
{
	if (str == "true" || str == "TRUE" || str == "1") return true;
	else return false;
}