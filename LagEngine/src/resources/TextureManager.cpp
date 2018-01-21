#include "TextureManager.h"

#include "../io/tinyxml/tinyxml.h"
#include "../renderer/graphicsAPI/Texture.h"

using namespace Lag;

TextureManager::TextureManager(TextureBuilder *builder) :
	XmlResourceManager("TextureManager", builder)
{
}

TextureBuilder::TextureBuilder(const TiXmlDocument &resourcesXml) :
	XmlResourceBuilder("texture", resourcesXml, ""), //Textures don't need folder path
	buildingFromXml(true)
{
}

void TextureBuilder::setBuildCustom(const ImageData &imageData, const TextureData &textureData)
{
	this->imageData = imageData;
	this->textureData = textureData;
	buildingFromXml = false;
}

TextureData TextureBuilder::parseTextureData(const TiXmlElement &element)
{
	TextureData data;

	const char* typeStr = element.Attribute("type");
	if (!typeStr)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "TextureBuilder",
			"A <texture> element on the Resources file does not contain the required element <type>");
		return data;
	}

	data.type = parseTextureType(typeStr);
	data.dataType = LAG_TEXTURE_DATA_TYPE_COLOR; //loading depth or stencil from an image makes no sense
	
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

	return data;
}

std::vector<std::string> TextureBuilder::parseTextureImages(const TextureData &texturedata, const TiXmlElement &element)
{
	std::vector<std::string> imageNames;
	if (texturedata.type != LAG_TEXTURE_TYPE_CUBE)
	{
		const char* image = element.Attribute("image");
		if (!image)
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "TextureBuilder",
				"A <texture> element on the Resources file does not contain the required element <image>");
			return imageNames;
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
			LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "TextureBuilder",
				"A <texture> element on the Resources file does not contain all required elements: <image[+-XYZ]>");
			return imageNames;
		}

		imageNames.push_back(imageX);
		imageNames.push_back(imageNegX);
		imageNames.push_back(imageY);
		imageNames.push_back(imageNegY);
		imageNames.push_back(imageZ);
		imageNames.push_back(imageNegZ);
	}
	return imageNames;
}

TextureType TextureBuilder::parseTextureType(const std::string &type)
{
	if (type == "1D") return LAG_TEXTURE_TYPE_1D;
	else if (type == "2D") return LAG_TEXTURE_TYPE_2D;
	else if (type == "Cube") return LAG_TEXTURE_TYPE_CUBE;
	//else if (type == "3D") return LAG_TEXTURE_TYPE_3D;
	else return LAG_TEXTURE_TYPE_2D;
}

TextureSemantic TextureBuilder::parseSemantic(const std::string &sem)
{
	if (sem == "Diffuse" || sem == "DiffuseColor") return LAG_TEXTURE_SEMANTIC_DIFFUSE;
	else if (sem == "Normal")return LAG_TEXTURE_SEMANTIC_NORMAL;
	else return LAG_TEXTURE_SEMANTIC_CUSTOM;
}

TextureFilteringMode TextureBuilder::parseFilteringMode(const std::string &mode)
{
	if (mode == "Nearest") return LAG_TEXTURE_FILTERING_MODE_NEAREST;
	else if (mode == "Linear") return LAG_TEXTURE_FILTERING_MODE_LINEAR;
	else if (mode == "NearestMipmapNearest") return LAG_TEXTURE_FILTERING_MODE_NEAREST_MIPMAP_NEAREST;
	else if (mode == "NearestMipmapLinear") return LAG_TEXTURE_FILTERING_MODE_NEAREST_MIPMAP_LINEAR;
	else if (mode == "LinearMipmapNearest") return LAG_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_NEAREST;
	else if (mode == "LinearMipmapLinear" || mode == "TriLinear") return LAG_TEXTURE_FILTERING_MODE_LINEAR_MIPMAP_LINEAR;
	else return LAG_TEXTURE_FILTERING_MODE_LINEAR;
}

TexturewWrappingMode TextureBuilder::parseWrappingMode(const std::string &mode)
{
	if (mode == "Repeat") return LAG_TEXTURE_WRAPPING_MODE_REPEAT;
	else if (mode == "MirroredRepeat") return LAG_TEXTURE_WRAPPING_MODE_MIRRORED_REPEAT;
	else if (mode == "ClampEdge" || mode == "ClampToEdge") return LAG_TEXTURE_WRAPPING_MODE_CLAMP_TO_EDGE;
	else if (mode == "ClampBorder" || mode == "ClampToBorder") return LAG_TEXTURE_WRAPPING_MODE_CLAMP_TO_BORDER;
	else return LAG_TEXTURE_WRAPPING_MODE_REPEAT;
}

int TextureBuilder::parseInt(const std::string &str)
{
	return std::stoi(str);
}