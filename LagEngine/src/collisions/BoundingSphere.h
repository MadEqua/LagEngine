#pragma once

#include "IBoundingVolume.h"

namespace Lag {
    class BoundingSphere : public IBoundingVolume {
    public:
        BoundingSphere();
        BoundingSphere(const BoundingSphere &other);

        //Will enclose all points.
        explicit BoundingSphere(const std::vector<glm::vec3> &points);

        //Will enclose the other transformed BoundingSphere.
        BoundingSphere(const BoundingSphere &other, const glm::mat4 &transform);

        std::unique_ptr<IBoundingVolume> clone() const override;

        void empty() override;
        void enclose(const glm::vec3 &point) override;

        void transform(const glm::mat4 &transform) override;

        inline float getRadius() const { return radius; }
        inline glm::vec3 getCenter() const { return center; }

    private:
        glm::vec3 center;
        float radius;
    };
};