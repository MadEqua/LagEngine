#include "IndexData.h"

using namespace Lag;

uint8 IndexData::getTypeByteSize() const {
    switch (indexType) {
        case IndexType::UINT8:
            return 1;
        case IndexType::UINT16:
            return 2;
        case IndexType::UINT32:
            return 4;
        default:
            return 0;
    }
}