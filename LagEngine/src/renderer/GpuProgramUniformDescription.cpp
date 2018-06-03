#include "GpuProgramUniformDescription.h"

using namespace Lag;

uint8 GpuProgramUniformDescription::getSize() const {
    switch (size) {
        case GpuProgramUniformSize::SIZE_1:
            return 1;
        case GpuProgramUniformSize::SIZE_2:
            return 2;
        case GpuProgramUniformSize::SIZE_3:
            return 3;
        case GpuProgramUniformSize::SIZE_4:
            return 4;
        default:
            return 0;
    }
}

uint8 GpuProgramUniformDescription::getSizeBytes() const {
    uint8 size = getSize();

    switch (type) {
        case GpuProgramUniformType::BOOL:
            return sizeof(bool) * size;
        case GpuProgramUniformType::FLOAT:
            return sizeof(float) * size;
        case GpuProgramUniformType::INT32:
            return sizeof(int32) * size;
        case GpuProgramUniformType::UINT32:
            return sizeof(uint32) * size;
        case GpuProgramUniformType::MATRIX:
            return sizeof(float) * size * size;
        default:
            return 0;
    }
}