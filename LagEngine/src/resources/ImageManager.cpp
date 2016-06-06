#include "ImageManager.h"

#include "../io/tinyxml/tinyxml.h"
#include "Image.h"

using namespace Lag;

ImageManager::ImageManager() :
	ResourceManager("ImageManager")
{
}

ImageManager::~ImageManager()
{
}

bool ImageManager::create(const std::string &name, const std::string &file, const ImageData &data)
{
	return add(name, new Image(file, data));
}

void ImageManager::parseResourceDescription(const TiXmlElement &element)
{
	if (element.ValueStr() == "image")
	{
		const char* name = element.Attribute("name");
		const char* file = element.Attribute("file");

		if (!name || !file)
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "ImageManager",
				"A <image> element on the Resources file does not contain all required elements: <name> and <file>");
			return;
		}
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

		create(name, file, data);
	}
}

ImageComponents ImageManager::parseComponents(const std::string &c) const
{
	if(c == "R") return LAG_IMAGE_COMPONENTS_R;
	else if (c == "RG") return LAG_IMAGE_COMPONENTS_RG;
	else if (c == "RGB") return LAG_IMAGE_COMPONENTS_RGB;
	else if (c == "RGBA") return LAG_IMAGE_COMPONENTS_RGBA;
	else return LAG_IMAGE_COMPONENTS_RGB;
}

ImageComponentType ImageManager::parseComponentType(const std::string &type) const
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

bool ImageManager::parseBool(const std::string &str) const
{
	if (str == "true" || str == "TRUE" || str == "1") return true;
	else return false;
}

int ImageManager::parseInt(const std::string &str) const
{
	return std::stoi(str);
}