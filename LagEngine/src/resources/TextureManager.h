#pragma once

#include "XmlResourceManager.h"
#include "../renderer/graphicsAPI/Texture.h"
#include <string>

namespace Lag
{
	enum TextureType;
	struct TextureData;
	enum TextureSemantic;
	enum TextureFilteringMode;
	enum TexturewWrappingMode;
	struct ImageData;

	class TextureBuilder : public XmlResourceBuilder<Texture>
	{
	public:
		TextureBuilder(const TiXmlDocument &resourcesXml);

		inline void setBuildFromXml() { buildingFromXml = true; }
		void setBuildCustom(const ImageData &imageData, const TextureData &textureData);

	protected:
		static TextureData parseTextureData(const TiXmlElement &element);
		static std::vector<std::string> parseTextureImages(const TextureData &texturedata, const TiXmlElement &element);

		static TextureType parseTextureType(const std::string &type);
		static TextureSemantic parseSemantic(const std::string &sem);
		static TextureFilteringMode parseFilteringMode(const std::string &mode);
		static TexturewWrappingMode parseWrappingMode(const std::string &mode);
		static int parseInt(const std::string &str);

		bool buildingFromXml;
		ImageData imageData;
		TextureData textureData;
	};

	
	class TextureManager : public XmlResourceManager<Texture>
	{
	public:
		explicit TextureManager(TextureBuilder *builder);
		virtual void initialize() override;
	};
}