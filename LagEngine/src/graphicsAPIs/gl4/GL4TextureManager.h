#pragma once

#include "../../resources/TextureManager.h"
#include "GL4Texture.h"
#include <string>

namespace Lag
{
	class GL4TextureBuilder : public TextureBuilder
	{
	public:
		GL4TextureBuilder::GL4TextureBuilder(const TiXmlDocument &resourcesXml) :
			TextureBuilder(resourcesXml) {}

		virtual Texture* build(const std::string &name) const override
		{
			if (buildingFromXml)
				return XmlResourceBuilder::build(name);
			else
				return new GL4Texture(imageData, textureData);
		}

		virtual Texture* parseAndCreate(const std::string &name, const TiXmlElement &element) const override
		{
			TextureData textureData = parseTextureData(element);
			return new GL4Texture(parseTextureImages(textureData, element), textureData);
		}
	};


	class GL4TextureManager : public TextureManager
	{
	public:
		explicit GL4TextureManager(TextureBuilder *builder) :
			TextureManager(builder) {}
	};
}