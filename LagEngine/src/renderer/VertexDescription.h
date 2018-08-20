#pragma once

#include "Types.h"

#include <vector>

namespace Lag {
    //The order of this enum is important.
    //It's the same order that will be expected (and sent to) shaders.
    enum class VertexAttributeSemantic : uint8 {
        POSITION,
        NORMAL,
        TANGENT,
        BINORMAL,
        TEXCOORD,
        COLOR
    };

    enum class VertexAttributeType : uint8 {
        HALF_FLOAT,
        FLOAT,
        FIXED, //16-bit fixed-point two's complement value
        INT8,
        UINT8,
        INT16,
        UINT16,
        INT32,
        UINT32
    };

    /*
    * An attribute of a vertex. Contained on a VertexDescription.
    */
    class VertexAttribute {
    public:
        inline VertexAttributeSemantic getSemantic() const { return semantic; }
        inline uint32 getOffset() const { return offset; }
        inline uint8 getLength() const { return length; }
        inline VertexAttributeType getType() const { return type; }
        inline uint8 getIndex() const { return index; }
        inline bool getIsNormalized() const { return isNormalized; }

        uint16 getByteSize() const;
        uint8 getTypeByteSize() const;

        bool operator==(const VertexAttribute &other) const;
        explicit operator std::size_t() const;
        bool operator<(const VertexAttribute &other) const;

    private:
        //Only VertexDescriptions can construct VertexAttributes
        friend class VertexDescription;

        VertexAttribute(VertexAttributeSemantic semantic, uint32 offset, uint8 length,
                        VertexAttributeType type, uint8 index, bool isNormalized);

        VertexAttributeSemantic semantic;
        uint32 offset; //How far the attribute is offset from the start of a vertex (bytes)
        uint8 length; //Ex: a position usually has length 3 (x,y,z)
        VertexAttributeType type;
        uint8 index; //Allowing several attributes with the same semantic (ex: texCoord1, texCoord2)
        bool isNormalized;

        //used to sort a list of attributes
        uint8 sortKey;
    };


    /*
    * Describes a vertex stored on a vertex buffer. (All the attributes of a vertex)
    */
    class VertexDescription {
    public:
        inline uint32 getAttributeCount() const { return static_cast<uint32>(attributes.size()); }
        inline const VertexAttribute &getAttribute(uint32 index) const { return attributes[index]; }
        const VertexAttribute *getAttribute(VertexAttributeSemantic semantic, uint8 index = 0) const;

        //The size of a full vertex
        uint32 getVertexByteSize() const;

        //The sum of  the lengths of all attributes
        uint32 getVertexLength() const;

        void addAttribute(VertexAttributeSemantic semantic, uint8 length, VertexAttributeType type, uint8 index = 0,
                          bool isNormalized = false);

        bool operator==(const VertexDescription &other) const;
        explicit operator std::size_t() const;

    private:
        std::vector<VertexAttribute> attributes;

        //Only InputDescriptionManager can construct VertexDescriptions
        friend class InputDescriptionManager;
    };
}