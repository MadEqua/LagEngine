#include "TextureManager.h"

#include "tinyxml/tinyxml.h"
#include "Texture.h"

#include "Constants.h"

using namespace Lag;

TextureManager::TextureManager(TextureBuilder *builder) :
	XmlResourceManager("TextureManager", builder)
{
}

void TextureManager::initializeFallbackObject()
{
	fallbackObject = get("defaultTexture");
}

std::vector<std::string> TextureManager::getTexturesWithImageName(const std::string &imageName) const
{
	std::vector<std::string> res;

	for (auto &pair : objects)
	{
		Texture &tex = *pair.second;
		if (tex.containsImageWithName(imageName))
			res.push_back(pair.first);
	}
	return res;
}

TextureBuilder::TextureBuilder(const XmlResourceBuilderData &xmlResourceData) :
	XmlResourceBuilder(xmlResourceData), //Textures don't need folder path
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
		LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "TextureBuilder",
			"A <texture> element on the Resources file does not contain the required element <type>");
		return data;
	}

	data.type = parseTextureType(typeStr);
	data.dataType = TextureDataType::COLOR; //loading depth or stencil from an image makes no sense
	
	const char* mipMapsStr = element.Attribute("mipmaps");
	const char* semanticStr = element.Attribute("semantic");
	const char* minStr = element.Attribute("minFilter");
	const char* magStr = element.Attribute("magFilter");

	const char* wrappingAllStr = element.Attribute("wrappingAll");
	const char* wrappingSStr = element.Attribute("wrappingS");
	const char* wrappingTStr = element.Attribute("wrappingT");
	const char* wrappingRStr = element.Attribute("wrappingR");

	if (mipMapsStr != nullptr)
		data.mipmaps = static_cast<uint32>(parseInt(mipMapsStr));
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
	if (texturedata.type != TextureType::TYPE_3D)
	{
		const char* image = element.Attribute(IMAGE_XML_TAG);
		if (!image)
		{
			LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "TextureBuilder",
				"A <texture> element on the Resources file does not contain the required element <image>");
			return imageNames;
		}
		imageNames.emplace_back(image);
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
			LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "TextureBuilder",
				"A <texture> element on the Resources file does not contain all required elements: <image[+-XYZ]>");
			return imageNames;
		}

		imageNames.emplace_back(imageX);
		imageNames.emplace_back(imageNegX);
		imageNames.emplace_back(imageY);
		imageNames.emplace_back(imageNegY);
		imageNames.emplace_back(imageZ);
		imageNames.emplace_back(imageNegZ);
	}
	return imageNames;
}

TextureType TextureBuilder::parseTextureType(const std::string &type)
{
	if (type == "1D") return TextureType::TYPE_1D;
	else if (type == "2D") return TextureType::TYPE_2D;
	else if (type == "Cube") return TextureType::TYPE_3D;
	//else if (type == "3D") return LAG_TEXTURE_TYPE_3D;
	else return TextureType::TYPE_2D;
}

TextureSemantic TextureBuilder::parseSemantic(const std::string &sem)
{
	if (sem == "Diffuse" || sem == "DiffuseColor") return TextureSemantic::DIFFUSE;
	else if (sem == "Normal")return TextureSemantic::NORMAL;
	else return TextureSemantic::CUSTOM;
}

TextureFilteringMode TextureBuilder::parseFilteringMode(const std::string &mode)
{
	if (mode == "Nearest") return TextureFilteringMode::NEAREST;
	else if (mode == "Linear") return TextureFilteringMode::LINEAR;
	else if (mode == "NearestMipmapNearest") return TextureFilteringMode::NEAREST_MIPMAP_NEAREST;
	else if (mode == "NearestMipmapLinear") return TextureFilteringMode::NEAREST_MIPMAP_LINEAR;
	else if (mode == "LinearMipmapNearest") return TextureFilteringMode::LINEAR_MIPMAP_NEAREST;
	else if (mode == "LinearMipmapLinear" || mode == "TriLinear") return TextureFilteringMode::LINEAR_MIPMAP_LINEAR;
	else return TextureFilteringMode::LINEAR;
}

TexturewWrappingMode TextureBuilder::parseWrappingMode(const std::string &mode)
{
	if (mode == "Repeat") return TexturewWrappingMode ::REPEAT;
	else if (mode == "MirroredRepeat") return TexturewWrappingMode::MIRRORED_REPEAT;
	else if (mode == "ClampEdge" || mode == "ClampToEdge") return TexturewWrappingMode::CLAMP_TO_EDGE;
	else if (mode == "ClampBorder" || mode == "ClampToBorder") return TexturewWrappingMode::CLAMP_TO_BORDER;
	else return TexturewWrappingMode::REPEAT;
}

int TextureBuilder::parseInt(const std::string &str)
{
	return std::stoi(str);
}