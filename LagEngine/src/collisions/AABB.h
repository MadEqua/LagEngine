#pragma once

#include "IBoundingVolume.h"

namespace Lag {
    class AABB : public IBoundingVolume {
    public:
        AABB();
        AABB(const AABB &other);

        //Will enclose all points.
        explicit AABB(const std::vector<glm::vec3> &points);

        //Will enclose the other transformed AABB.
        AABB(const AABB &other, const glm::mat4 &transform);

        std::unique_ptr<IBoundingVolume> clone() const override;

        void empty() override;
        void enclose(const glm::vec3 &point) override;

        void transform(const glm::mat4 &transform) override;

        inline glm::vec3 getMin() const { return glm::vec3(min[0], min[1], min[2]); }
        inline glm::vec3 getMax() const { return glm::vec3(max[0], max[1], max[2]); }
        inline glm::vec3 getDimensions() const { return getMax() - getMin(); }
        inline glm::vec3 getCenter() const { return (getMin() + getMax()) / 2.0f; }

    private:
        float min[3];
        float max[3];
    };
};