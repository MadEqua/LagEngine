#include "TextureManager.h"

#include "../io/tinyxml/tinyxml.h"
#include "../renderer/graphicsAPI/Texture.h"

using namespace Lag;

TextureManager::TextureManager() :
	ResourceManager("TextureManager")
{
}

void TextureManager::parseResourceDescription(const TiXmlElement &element)
{
	if (element.ValueStr() == "texture")
	{
		const char* name = element.Attribute("name");
		const char* typeStr = element.Attribute("type");
		if (!name || !typeStr)
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "TextureManager",
				"A <texture> element on the Resources file does not contain all required elements: <name> and <type>");
			return;
		}

		TextureData data;
		data.type = parseTextureType(typeStr);
		data.dataType = LAG_TEXTURE_DATA_TYPE_COLOR; //loading depth or stencil from an image makes no sense
		std::vector<std::string> imageNames;

		if (data.type != LAG_TEXTURE_TYPE_CUBE)
		{
			const char* image = element.Attribute("image");
			if (!image)
			{
				LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "TextureManager",
					"A <texture> element on the Resources file does not contain all required elements: <image>");
				return;
			}
			imageNames.push_back(image);
		}
		else
		{
			const char* imageX = element.Attribute("imageX");
			const char* imageNegX = element.Attribute("image-X");
			const char* imageY = element.Attribute("imageY");
			const char* imageNegY = element.Attribute("image-Y");
			const char* imageZ = element.Attribute("imageZ");
			const char* imageNegZ = element.Attribute("image-Z");

			if (!imageX || !imageNegX || !imageY || !imageNegY || !imageZ || !imageNegZ)
			{
				LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "TextureManager",
					"A <texture> element on the Resources file does not contain all required elements: <image[+-XYZ]>");
				return;
			}

			imageNames.push_back(imageX);
			imageNames.push_back(imageNegX);
			imageNames.push_back(imageY);
			imageNames.push_back(imageNegY);
			imageNames.push_back(imageZ);
			imageNames.push_back(imageNegZ);
		}

		const char* mipMapsStr = element.Attribute("mipmaps");
		const char* semanticStr = element.Attribute("semantic");
		const char* minStr = element.Attribute("minFilter");
		const char* magStr = element.Attribute("magFilter");

		const char* wrappingAllStr = element.Attribute("wrappingAll");
		const char* wrappingSStr = element.Attribute("wrappingS");
		const char* wrappingTStr = element.Attribute("wrappingT");
		const char* wrappingRStr = element.Attribute("wrappingR");

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

		create(name, imageNames, data);
	}
}

TextureType TextureManager::parseTextureType(const std::string &type) const
{
	if (type == "1D") return LAG_TEXTURE_TYPE_1D;
	else if (type == "2D") return LAG_TEXTURE_TYPE_2D;
	else if (type == "Cube") return LAG_TEXTURE_TYPE_CUBE;
	//else if (type == "3D") return LAG_TEXTURE_TYPE_3D;
	else return LAG_TEXTURE_TYPE_2D;
}

TextureSemantic TextureManager::parseSemantic(const std::string &sem) const
{
	if (sem == "Diffuse" || sem == "DiffuseColor") return LAG_TEXTURE_SEMANTIC_DIFFUSE;
	else if (sem == "Normal")return LAG_TEXTURE_SEMANTIC_NORMAL;
	else return LAG_TEXTURE_SEMANTIC_CUSTOM;
}

TextureFilteringMode TextureManager::parseFilteringMode(const std::string &mode) const
{
	if (mode == "Nearest") return LAG_TEXTURE_FILTERING_MODE_NEAREST;
	else if (mode == "Linear") return LAG_TEXTURE_FILTERING_MODE_LINEAR;
	else if (mode == "NearestMipmapNearest") return LAG_TEXTURE_FILTERING_MODE_NEAREST_MIPMAP_NEAREST;
	else if (mode == "NearestMipmapLinear") return LAG_TEXTURE_FILTERING_MODE_NEAREST_MIPMAP_LINEAR;
	else if (mode == "LinearMipmapNearest") return LAG_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_NEAREST;
	else if (mode == "LinearMipmapLinear" || mode == "TriLinear") return LAG_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_LINEAR;
	else return LAG_TEXTURE_FILTERING_MODE_LINEAR;
}

TexturewWrappingMode TextureManager::parseWrappingMode(const std::string &mode) const
{
	if (mode == "Repeat") return LAG_TEXTURE_WRAPPING_MODE_REPEAT;
	else if (mode == "MirroredRepeat") return LAG_TEXTURE_WRAPPING_MODE_MIRRORED_REPEAT;
	else if (mode == "ClampEdge" || mode == "ClampToEdge") return LAG_TEXTURE_WRAPPING_MODE_CLAMP_TO_EDGE;
	else if (mode == "ClampBorder" || mode == "ClampToBorder") return LAG_TEXTURE_WRAPPING_MODE_CLAMP_TO_BORDER;
	else return LAG_TEXTURE_WRAPPING_MODE_REPEAT;
}

int TextureManager::parseInt(const std::string &str) const
{
	return std::stoi(str);
}