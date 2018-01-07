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
	
	class TextureManager : public XmlResourceManager<Texture>
	{
	public:
		TextureManager();

		bool create(const std::string &name, const std::string &imageName, const TextureData &data);
		bool create(const std::string &name, const std::vector<std::string> &imageNames, const TextureData &data);
		bool create(const std::string &name, const ImageData &imageData, const TextureData &textureData);

	protected:
		virtual Texture* internalCreate(const std::string &imageName, const TextureData &data) = 0;
		virtual Texture* internalCreate(const std::vector<std::string> &imageNames, const TextureData &data) = 0;
		virtual Texture* internalCreate(const ImageData &imageData, const TextureData &textureData) = 0;

		virtual void parseResourceDescription(const TiXmlElement &element) override;

		TextureType parseTextureType(const std::string &type) const;
		TextureSemantic parseSemantic(const std::string &sem) const;
		TextureFilteringMode parseFilteringMode(const std::string &mode) const;
		TexturewWrappingMode parseWrappingMode(const std::string &mode) const;
		int parseInt(const std::string &str) const;
	};
}