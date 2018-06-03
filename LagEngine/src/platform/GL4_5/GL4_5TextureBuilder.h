#pragma once

#include "TextureManager.h"
#include "GL4_5Texture.h"
#include <string>

namespace Lag {
    class GL4_5TextureBuilder : public TextureBuilder {
    public:
        explicit GL4_5TextureBuilder(const XmlResourceBuilderData &xmlResourceData) :
                TextureBuilder(xmlResourceData) {}

        Texture *build(const std::string &name) const override {
            if (buildingFromXml)
                return XmlResourceBuilder::build(name);
            else
                return new GL4_5Texture(imageData, textureData);
        }

        Texture *parseAndCreate(const std::string &path, const TiXmlElement &element) const override {
            TextureData textureData = parseTextureData(element);
            return new GL4_5Texture(parseTextureImages(textureData, element), textureData);
        }
    };
}