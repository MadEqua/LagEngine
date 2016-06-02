#pragma once

#include "ResourceManager.h"
#include <string>

namespace Lag
{
	enum TextureType;
	enum TextureComponents;
	enum TextureComponentType;
	struct TextureData;
	enum TextureSemantic;
	
	class TextureManager : public ResourceManager
	{
	public:
		TextureManager();
		~TextureManager();

		virtual bool create(const std::string &name, const std::string &file, 
			TextureType type, const TextureData &data) = 0;

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;

		TextureType parseTextureType(const std::string &type) const;
		TextureComponents parseComponents(const std::string &c) const;
		TextureComponentType parseComponentType(const std::string &type) const;
		TextureSemantic parseSemantic(const std::string &sem) const;
		bool parseBool(const std::string &str) const;
		int parseInt(const std::string &str) const;
	};
}