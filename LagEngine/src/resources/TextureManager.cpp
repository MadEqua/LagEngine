#include "TextureManager.h"

#include "../io/tinyxml/tinyxml.h"
#include "../renderer/graphicsAPI/Texture.h"

using namespace Lag;

TextureManager::TextureManager() :
	ResourceManager("TextureManager")
{
}

TextureManager::~TextureManager() 
{
}

void TextureManager::parseResourceDescription(const TiXmlElement &element)
{
	if (element.ValueStr() == "texture")
	{
		const char* name = element.Attribute("name");
		const char* file = element.Attribute("file");

		if (!name || !file)
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "TextureManager",
				"A <texture> element on the Resources file does not contain all required elements: <name> and <file>");
			return;
		}

		const char* typeStr = element.Attribute("type");
		TextureType type = LAG_TEXTURE_TYPE_2D;

		if (typeStr != nullptr)
			type = parseTextureType(typeStr);

		const char* componentsStr = element.Attribute("components");
		const char* componentTypeStr = element.Attribute("componentType");
		const char* normalizedStr = element.Attribute("normalized");
		const char* sRgbStr = element.Attribute("sRGB");
		const char* mipMapsStr = element.Attribute("mipmaps");

		TextureData format;
		if (componentsStr != nullptr)
			format.components = parseComponents(componentsStr);
		if (componentTypeStr != nullptr)
			format.componentType = parseComponentType(componentTypeStr);
		if (normalizedStr != nullptr)
			format.normalized = parseBool(normalizedStr);
		if (sRgbStr != nullptr)
			format.sRGB = parseBool(sRgbStr);
		if (mipMapsStr != nullptr)
			format.mipmaps = parseInt(mipMapsStr);

		create(name, file, type, format);
	}
}

TextureType TextureManager::parseTextureType(const std::string &type) const
{
	if (type == "1D") return LAG_TEXTURE_TYPE_1D;
	else if (type == "2D") return LAG_TEXTURE_TYPE_2D;
	//else if (type == "3D") return LAG_TEXTURE_TYPE_3D;
	else return LAG_TEXTURE_TYPE_2D;
}

TextureComponents TextureManager::parseComponents(const std::string &c) const
{
	if(c == "R") return LAG_TEXTURE_COMPONENTS_R;
	else if (c == "RG") return LAG_TEXTURE_COMPONENTS_RG;
	else if (c == "RGB") return LAG_TEXTURE_COMPONENTS_RGB;
	else if (c == "RGBA") return LAG_TEXTURE_COMPONENTS_RGBA;
	else return LAG_TEXTURE_COMPONENTS_RGB;
}

TextureComponentType TextureManager::parseComponentType(const std::string &type) const
{
	if(type == "FLOAT16" || type == "HALF_FLOAT") return LAG_TEXTURE_COMPONENT_TYPE_FLOAT16;
	else if (type == "FLOAT32" || type == "FLOAT") return LAG_TEXTURE_COMPONENT_TYPE_FLOAT32;
	
	else if (type == "INT8") return LAG_TEXTURE_COMPONENT_TYPE_INT8;
	else if (type == "INT16")return LAG_TEXTURE_COMPONENT_TYPE_INT16;
	else if (type == "INT32") return LAG_TEXTURE_COMPONENT_TYPE_INT32;

	else if (type == "UINT8") return LAG_TEXTURE_COMPONENT_TYPE_UINT8;
	else if (type == "UINT16")return LAG_TEXTURE_COMPONENT_TYPE_UINT16;
	else if (type == "UINT32") return LAG_TEXTURE_COMPONENT_TYPE_UINT32;

	else return LAG_TEXTURE_COMPONENT_TYPE_UINT8;
}

bool TextureManager::parseBool(const std::string &str) const
{
	if (str == "true" || str == "TRUE" || str == "1") return true;
	else return false;
}

int TextureManager::parseInt(const std::string &str) const
{
	return std::stoi(str);
}