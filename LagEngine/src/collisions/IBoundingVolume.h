#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <memory>

#include "IntersectionResult.h"

#include "Types.h"

namespace Lag {

    enum class BoundingVolumeType : uint8 {
        AABB, SPHERE
    };

    class IBoundingVolume {
    public:
        explicit IBoundingVolume(BoundingVolumeType type);
        virtual ~IBoundingVolume() = default;

        virtual std::unique_ptr<IBoundingVolume> clone() const = 0;

        virtual void empty() = 0;
        virtual void enclose(const glm::vec3 &point) = 0;

        //Transforms itself by the matrix and recompute the bounds.
        virtual void transform(const glm::mat4 &transform) = 0;

        void enclose(const IBoundingVolume &other);
        IntersectionResult intersects(const IBoundingVolume &other) const;

        inline BoundingVolumeType getType() const { return type; }

    protected:
        BoundingVolumeType type;
    };
};