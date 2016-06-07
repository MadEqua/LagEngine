#pragma once

#include "ResourceManager.h"
#include <string>

namespace Lag
{
	enum ImageComponents;
	enum ImageComponentType;
	struct ImageData;
	
	class ImageManager : public ResourceManager
	{
	public:
		ImageManager(const std::string &folder);
		~ImageManager();

		bool create(const std::string &name, const std::string &file, const ImageData &data);

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;

		ImageComponents parseComponents(const std::string &c) const;
		ImageComponentType parseComponentType(const std::string &type) const;
		bool parseBool(const std::string &str) const;
		int parseInt(const std::string &str) const;
	};
}