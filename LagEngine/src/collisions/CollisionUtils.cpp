#include "CollisionUtils.h"

#include "AABB.h"
#include "BoundingSphere.h"

using namespace Lag;

bool CollisionUtils::intersects(const AABB &aabb1, const AABB &aabb2) {
    glm::vec3 min1 = aabb1.getMin();
    glm::vec3 min2 = aabb2.getMin();

    glm::vec3 max1 = aabb1.getMax();
    glm::vec3 max2 = aabb2.getMax();

    if(min1.x >= max2.x) return false;
    if(min1.y >= max2.y) return false;
    if(min1.z >= max2.z) return false;

    if(max1.x <= min2.x) return false;
    if(max1.y <= min2.y) return false;
    if(max1.z <= min2.z) return false;

    return true;
}

bool CollisionUtils::intersects(const AABB &aabb, const BoundingSphere &sphere) {
    glm::vec3 closestPointToCenter = findClosestPointOnAABB(aabb, sphere.getCenter());
    glm::vec3 dif = sphere.getCenter() - closestPointToCenter;
    float distanceSq = dif.x * dif.x + dif.y * dif.y + dif.z * dif.z;
    float radius = sphere.getRadius();
    return distanceSq < radius * radius;
}

bool CollisionUtils::intersects(const BoundingSphere &sphere1, const BoundingSphere &sphere2) {
    glm::vec3 dif = sphere1.getCenter() - sphere2.getCenter();
    float distanceSq = dif.x * dif.x + dif.y * dif.y + dif.z * dif.z;
    float radiusSum = sphere1.getRadius() + sphere2.getRadius();
    return distanceSq < radiusSum * radiusSum;
}

void CollisionUtils::enclose(AABB &encloser, const AABB &enclosee) {
    encloser.enclose(enclosee.getMin());
    encloser.enclose(enclosee.getMax());
}

void CollisionUtils::enclose(AABB &encloser, const BoundingSphere &enclosee) {
    //TODO  (not used in any case)
}

void CollisionUtils::enclose(BoundingSphere &encloser, const AABB &enclosee) {
    //TODO  (not used in any case)
}

void CollisionUtils::enclose(BoundingSphere &encloser, const BoundingSphere &enclosee) {
    glm::vec3 centersVector = enclosee.getCenter() - encloser.getCenter();
    float l = glm::length(centersVector);

    glm::vec3 normalizedCentersVector;
    if(l < 0.001f) {
        normalizedCentersVector = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    else {
        normalizedCentersVector = centersVector / l;
    }

    glm::vec3 farPoint = enclosee.getCenter() + normalizedCentersVector * enclosee.getRadius();
    encloser.enclose(farPoint);
}

glm::vec3 CollisionUtils::findClosestPointOnAABB(const AABB &aabb, const glm::vec3 &p) {
    auto min = aabb.getMin();
    auto max = aabb.getMax();

    glm::vec3 result(p);

    if(p.x < min.x) result.x = min.x;
    else if(p.x > max.x) result.x = max.x;

    if(p.y < min.y) result.y = min.y;
    else if(p.y > max.y) result.y = max.y;

    if(p.z < min.z) result.z = min.z;
    else if(p.z > max.z) result.z = max.z;

    return result;
}
