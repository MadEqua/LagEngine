#pragma once

#include "XmlResourceManager.h"
#include "Image.h"
#include <string>

namespace Lag
{
	enum ImageComponents;
	enum ImageComponentType;
	struct ImageData;

	class ImageBuilder : public XmlResourceBuilder<Image>
	{
	public:
		ImageBuilder(const XmlResourceBuilderData &xmlResourceData);

		virtual Image* parseAndCreate(const std::string &path, const TiXmlElement &element) const override;

	protected:
		static ImageData parseImageData(const TiXmlElement &element);
		static ImageComponents parseComponents(const std::string &c);
		static ImageComponentType parseComponentType(const std::string &type);
		static bool parseBool(const std::string &str);
	};
	
	class ImageManager : public XmlResourceManager<Image>
	{
	public:
		explicit ImageManager(ImageBuilder* builder);
		virtual void initializeFallbackObject() override;
	};
}