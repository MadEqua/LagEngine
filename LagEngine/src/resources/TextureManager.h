#pragma once

#include "XmlResourceManager.h"
#include "Texture.h"

#include <string>
#include <vector>

namespace Lag {
    enum class TextureType : uint8;
    struct TextureData;
    enum class TextureSemantic : uint8;
    enum class TextureFilteringMode : uint8;
    enum class TexturewWrappingMode : uint8;
    struct ImageData;

    class TextureBuilder : public XmlResourceBuilder<Texture> {
    public:
        explicit TextureBuilder(const XmlResourceBuilderData &xmlResourceData);

        inline void setBuildFromXml() { buildingFromXml = true; }
        void setBuildCustom(const ImageData &imageData, const TextureData &textureData);

    protected:
        static TextureData parseTextureData(const TiXmlElement &element);
        static std::vector<std::string> parseTextureImages(const TextureData &texturedata, const TiXmlElement &element);

        static TextureType parseTextureType(const std::string &type);
        static TextureSemantic parseSemantic(const std::string &sem);
        static TextureFilteringMode parseFilteringMode(const std::string &mode);
        static TexturewWrappingMode parseWrappingMode(const std::string &mode);

        bool buildingFromXml;
        ImageData imageData;
        TextureData textureData;
    };


    class TextureManager : public XmlResourceManager<Texture> {
    public:
        explicit TextureManager(TextureBuilder *builder);
        Handle<Texture> getFallbackObject() override;

        std::vector<std::string> getTexturesWithImageName(const std::string &imageName) const;
    };
}