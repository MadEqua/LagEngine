#pragma once

#include "glm/glm.hpp"

namespace Lag {

    class AABB;
    class BoundingSphere;

    namespace CollisionUtils {

        bool intersects(const AABB &aabb1, const AABB &aabb2);
        bool intersects(const AABB &aabb, const BoundingSphere &sphere);
        bool intersects(const BoundingSphere &sphere1, const BoundingSphere &sphere2);

        void enclose(AABB &encloser, const AABB &enclosee);
        void enclose(AABB &encloser, const BoundingSphere &enclosee);
        void enclose(BoundingSphere &encloser, const AABB &enclosee);
        void enclose(BoundingSphere &encloser, const BoundingSphere &enclosee);

        //Find the closest point to p on the AABB
        //Returns p if already inside the box
        glm::vec3 findClosestPointOnAABB(const AABB &aabb, const glm::vec3 &p);
    }
}