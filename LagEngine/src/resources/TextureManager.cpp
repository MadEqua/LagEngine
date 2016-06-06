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
		const char* semanticStr = element.Attribute("semantic");
		const char* minStr = element.Attribute("minFilter");
		const char* magStr = element.Attribute("magFilter");

		const char* wrappingAllStr = element.Attribute("wrappingAll");
		const char* wrappingSStr = element.Attribute("wrappingS");
		const char* wrappingTStr = element.Attribute("wrappingT");
		const char* wrappingRStr = element.Attribute("wrappingR");


		TextureData data;
		if (componentsStr != nullptr)
			data.components = parseComponents(componentsStr);
		if (componentTypeStr != nullptr)
			data.componentType = parseComponentType(componentTypeStr);
		if (normalizedStr != nullptr)
			data.normalized = parseBool(normalizedStr);
		if (sRgbStr != nullptr)
			data.sRGB = parseBool(sRgbStr);
		if (mipMapsStr != nullptr)
			data.mipmaps = parseInt(mipMapsStr);
		if (semanticStr != nullptr)
			data.semantic = parseSemantic(semanticStr);
		if (minStr != nullptr)
			data.minificationFilteringMode = parseFilteringMode(minStr);
		if (magStr != nullptr)
			data.magnificationFilteringMode = parseFilteringMode(magStr);

		if (wrappingAllStr != nullptr)
		{
			data.wrappingMode[0] = data.wrappingMode[1] = data.wrappingMode[2] =
				parseWrappingMode(wrappingAllStr);
		}
		else
		{
			if (wrappingSStr != nullptr)
				data.wrappingMode[0] = parseWrappingMode(wrappingSStr);
			if (wrappingTStr != nullptr)
				data.wrappingMode[1] = parseWrappingMode(wrappingTStr);
			if (wrappingRStr != nullptr)
				data.wrappingMode[2] = parseWrappingMode(wrappingRStr);
		}

		create(name, file, type, data);
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

TextureSemantic TextureManager::parseSemantic(const std::string &sem) const
{
	if (sem == "Diffuse" || sem == "DiffuseColor") return LAG_TEXTURE_SEMANTIC_DIFFUSE;
	else if (sem == "Normal")return LAG_TEXTURE_SEMANTIC_NORMAL;
	else return LAG_TEXTURE_SEMANTIC_CUSTOM;
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

TextureFilteringMode TextureManager::parseFilteringMode(const std::string &mode) const
{
	if (mode == "Nearest") return LAG_TEXTURE_FILTERING_MODE_NEAREST;
	else if (mode == "Linear") return LAG_TEXTURE_FILTERING_MODE_LINEAR;
	else if (mode == "NearestMipmapNearest") return LAG_TEXTURE_FILTERING_MODE_NEAREST_MIPMAP_NEAREST;
	else if (mode == "NearestMipmapLinear")return LAG_TEXTURE_FILTERING_MODE_NEAREST_MIPMAP_LINEAR;
	else if (mode == "LinearMipmapNearest") return LAG_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_NEAREST;
	else if (mode == "LinearMipmapLinear" || mode == "TriLinear") return LAG_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_LINEAR;
	else return LAG_TEXTURE_FILTERING_MODE_LINEAR;
}

TexturewWrappingMode TextureManager::parseWrappingMode(const std::string &mode) const
{
	if (mode == "Repeat") return LAG_TEXTURE_WRAPPING_MODE_REPEAT;
	else if (mode == "MirroredRepeat") return LAG_TEXTURE_WRAPPING_MODE_MIRRORED_REPEAT;
	else if (mode == "ClampEdge") return LAG_TEXTURE_WRAPPING_MODE_CLAMP_TO_EDGE;
	else if (mode == "ClampBorder")return LAG_TEXTURE_WRAPPING_MODE_CLAMP_TO_BORDER;
	else return LAG_TEXTURE_WRAPPING_MODE_REPEAT;
}
