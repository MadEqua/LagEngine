#pragma once

#include "glm/glm.hpp"

#include <vector>

namespace Lag {
    class AABB {
    public:
        AABB();
        AABB(const AABB &other);

        //Will enclose all points.
        explicit AABB(const std::vector<glm::vec3> &points);

        //Will enclose the other transformed AABB.
        AABB(const AABB &other, const glm::mat4 &transform);

        void empty();
        void enclose(const glm::vec3 &point);
        void enclose(const AABB &other);

        //Transforms itself by the matrix and recompute the bounds.
        void transform(const glm::mat4 &transform);

        bool intersects(const AABB &other) const;

        inline glm::vec3 getMin() const { return glm::vec3(min[0], min[1], min[2]); }
        inline glm::vec3 getMax() const { return glm::vec3(max[0], max[1], max[2]); }
        inline glm::vec3 getDimensions() const { return getMax() + getMin(); }
        inline glm::vec3 getCenter() const { return (getMin() + getMax()) / 2.0f; }

    private:
        float min[3];
        float max[3];
    };
};