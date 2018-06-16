#pragma once

#include "XmlResource.h"
#include "Types.h"

#include <string>

namespace Lag {
    enum class ImageComponents : uint8 {
        RGBA,
        RGB,
        RG,
        R
    };

    enum class ImageComponentType : uint8 {
        FLOAT16,
        FLOAT32,

        INT8,
        INT16,
        INT32,

        UINT1,
        UINT4,
        UINT8,
        UINT16,
        UINT24,
        UINT32
    };

    struct ImageData {
        ImageData() : width(0), height(0),
                      components(ImageComponents::RGBA),
                      componentType(ImageComponentType::UINT8),
                      normalized(true),
                      sRGB(true) {}

        uint32 width, height;
        ImageComponents components;
        ImageComponentType componentType;
        bool normalized;
        bool sRGB; //is the image in non-linar format?
    };

    class Image : public XmlResource {
    public:
        inline const ImageData &getImageData() const { return imageData; }
        inline const byte *getRawDataPointer() const { return dataPtr; }

        bool loadImplementation() override;
        void unloadImplementation() override;

    protected:
        friend class ImageBuilder;

        Image(const std::string &path, const ImageData &imageData);

        ImageData imageData;
        byte *dataPtr;
    };
}