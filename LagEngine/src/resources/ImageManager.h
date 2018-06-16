#pragma once

#include "XmlResourceManager.h"
#include "Image.h"
#include "Types.h"

#include <string>

namespace Lag {
    enum class ImageComponents : uint8;
    enum class ImageComponentType : uint8;
    struct ImageData;

    class ImageBuilder : public XmlResourceBuilder<Image> {
    public:
        explicit ImageBuilder(const XmlResourceBuilderData &xmlResourceData);

        Image *parseAndCreate(const std::string &path, const TiXmlElement &element) const override;

    protected:
        static ImageData parseImageData(const TiXmlElement &element);
        static ImageComponents parseComponents(const std::string &c);
        static ImageComponentType parseComponentType(const std::string &type);
    };

    class ImageManager : public XmlResourceManager<Image> {
    public:
        explicit ImageManager(ImageBuilder *builder);
        void initializeFallbackObject() override;
    };
}