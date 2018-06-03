#pragma once

#include <string>
#include "Types.h"

namespace Lag {
    enum class GpuProgramUniformSize : uint8 {
        SIZE_1,
        SIZE_2,
        SIZE_3,
        SIZE_4,
        SIZE_UNKNOWN
    };

    enum class GpuProgramUniformType : uint8 {
        BOOL,
        FLOAT,
        INT32,
        UINT32,
        MATRIX,
        UNKNOWN
    };

    enum class GpuProgramUniformSemantic : uint8 {
        MODEL_MATRIX,
        MODELVIEW_MATRIX,
        VIEW_MATRIX,
        VIEWPROJECTION_MATRIX,
        MVP_MATRIX,
        PROJECTION_MATRIX,

        NORMAL_WORLD_MATRIX,
        NORMAL_VIEW_MATRIX,

        POINT_LIGHT_COUNT,
        POINT_LIGHT_POSITIONS,
        POINT_LIGHT_COLORS,
        POINT_LIGHT_ATTENUATIONS,

        DIR_LIGHT_COUNT,
        DIR_LIGHT_DIRECTIONS,
        DIR_LIGHT_COLORS,

        TEXTURE_DIFFUSE,
        TEXTURE_NORMAL,
        TEXTURE_CUSTOM,

        CUSTOM
    };

    /*
    * Container for all the data that describe an Uniform. Belongs to a GpuProgramStage.
    */
    struct GpuProgramUniformDescription {
        std::string name;
        GpuProgramUniformSize size;
        GpuProgramUniformType type;
        GpuProgramUniformSemantic semantic;

        uint8 getSize() const;
        uint8 getSizeBytes() const;
    };
}