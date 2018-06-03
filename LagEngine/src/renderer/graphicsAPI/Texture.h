#pragma once

#include "Handle.h"
#include "XmlResource.h"
#include "Types.h"
#include "Image.h"

#include <string>
#include <vector>
#include <iostream>

namespace Lag {
    //For shader uniform matching
    enum class TextureSemantic : uint8 {
        DIFFUSE,
        NORMAL,
        CUSTOM
    };

    enum class TextureType : uint8 {
        TYPE_1D,
        TYPE_2D,
        TYPE_3D,
    };

    enum class TextureDataType : uint8 {
        COLOR,
        DEPTH,
        STENCIL,
        DEPTH_STENCIL
    };

    enum class TextureFilteringMode : uint8 {
        NEAREST,
        LINEAR,

        //[inside the mipmap]_MIPMAP_[between mipmaps]
                NEAREST_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_NEAREST,
        LINEAR_MIPMAP_LINEAR
    };

    enum class TexturewWrappingMode : uint8 {
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER
    };

    struct TextureData {
        TextureData() :
                dataType(TextureDataType::COLOR),
                type(TextureType::TYPE_2D),
                semantic(TextureSemantic::DIFFUSE),
                minificationFilteringMode(TextureFilteringMode::LINEAR_MIPMAP_LINEAR),
                magnificationFilteringMode(TextureFilteringMode::LINEAR),
                wrappingMode{TexturewWrappingMode::REPEAT, TexturewWrappingMode::REPEAT, TexturewWrappingMode::REPEAT},
                mipmaps(8) {}

        TextureDataType dataType;
        TextureType type;
        TextureSemantic semantic;
        TextureFilteringMode minificationFilteringMode;
        TextureFilteringMode magnificationFilteringMode;
        TexturewWrappingMode wrappingMode[3];
        uint32 mipmaps;
    };

    /*
    * Gpu representation of an Image (or a set of images). May contain mipmaps.
    */
    class Texture : public XmlResource {
    public:
        inline const TextureData &getTextureData() const { return textureData; }
        inline const ImageData &getImageData() const { return imageData; }

        bool containsImageWithName(const std::string &imageName) const;

        //TODO: set texture parameter methods

    protected:
        Texture(const std::string &imageName, const TextureData &data);
        Texture(const std::vector<std::string> &imageNames, const TextureData &data);
        Texture(const ImageData &imageData, const TextureData &textureData);

        TextureData textureData;
        ImageData imageData;

        std::vector<std::string> imageNames;
        std::vector<Handle<Image>> images;

        bool loadImplementation() override;
        void unloadImplementation() override;
    };
}