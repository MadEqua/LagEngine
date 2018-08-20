#include "VertexDescription.h"

using namespace Lag;

VertexAttribute::VertexAttribute(VertexAttributeSemantic semantic, uint32 offset, uint8 length,
                                 VertexAttributeType type, uint8 index, bool isNormalized) :
        semantic(semantic),
        offset(offset),
        length(length),
        type(type),
        index(index),
        isNormalized(isNormalized) {
    auto sem = static_cast<uint8>(semantic);
    sortKey = (sem << 4U) | index;
}

uint16 VertexAttribute::getByteSize() const {
    return getTypeByteSize() * length;
}

uint8 VertexAttribute::getTypeByteSize() const {
    switch (type) {
        case VertexAttributeType::INT8:
        case VertexAttributeType::UINT8:
            return 1;
        case VertexAttributeType::HALF_FLOAT:
        case VertexAttributeType::FIXED:
        case VertexAttributeType::INT16:
        case VertexAttributeType::UINT16:
            return 2;
        case VertexAttributeType::FLOAT:
        case VertexAttributeType::INT32:
        case VertexAttributeType::UINT32:
            return 4;
        default:
            return 0;
    }
}

bool VertexAttribute::operator==(const VertexAttribute &other) const {
    return semantic == other.semantic &&
           offset == other.offset &&
           length == other.length &&
           type == other.type &&
           index == other.index &&
           isNormalized == other.isNormalized;
}

VertexAttribute::operator std::size_t() const {
    return static_cast<uint32>(semantic) ^
           offset ^
           static_cast<uint32>(length) ^
           static_cast<uint32>(type) ^
           static_cast<uint32>(index) ^
           static_cast<uint32>(isNormalized);
}

bool VertexAttribute::operator<(const VertexAttribute &other) const {
    return sortKey < other.sortKey;
}

///////////////////////////////////////
///////////////////////////////////////
void VertexDescription::addAttribute(VertexAttributeSemantic semantic, uint8 length, VertexAttributeType type, uint8 index,
                                bool isNormalized) {
    //Offset computation is below, after sorting
    VertexAttribute newAttr(semantic, 0, length, type, index, isNormalized);
    attributes.push_back(newAttr);

    std::sort(attributes.begin(), attributes.end());

    uint32 sum = 0;
    for (auto &attr : attributes) {
        attr.offset = sum;
        sum += attr.getByteSize();
    }
}

const VertexAttribute *VertexDescription::getAttribute(VertexAttributeSemantic semantic, uint8 index) const {
    for (const VertexAttribute &va : attributes)
        if (va.getSemantic() == semantic &&
            va.getIndex() == index)
            return &va;

    return nullptr;
}

uint32 VertexDescription::getVertexByteSize() const {
    uint32 size = 0;
    for (const VertexAttribute &attr : attributes)
        size += attr.getByteSize();
    return size;
}

uint32 VertexDescription::getVertexLength() const {
    uint32 length = 0;
    for (const VertexAttribute &attr : attributes)
        length += attr.getLength();
    return length;
}

bool VertexDescription::operator==(const VertexDescription &other) const {
    //Two VertexDescriptions are the same if they contain the same attributes, even in different orders.
    for (const VertexAttribute &attr : attributes) {
        auto it = std::find(other.attributes.begin(), other.attributes.end(), attr);
        if (it == other.attributes.end())
            return false;
    }

    return true;
}

VertexDescription::operator std::size_t() const {
    std::size_t ret = 0;
    for (const VertexAttribute &attr : attributes)
        ret ^= static_cast<std::size_t>(attr);
    return ret;
}
