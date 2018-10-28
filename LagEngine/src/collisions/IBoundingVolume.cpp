#include "IBoundingVolume.h"

#include "AABB.h"
#include "BoundingSphere.h"
#include "CollisionUtils.h"

using namespace Lag;

IBoundingVolume::IBoundingVolume(BoundingVolumeType type) :
    type(type) {
}

void IBoundingVolume::enclose(const IBoundingVolume &other) {
    if(type == BoundingVolumeType::AABB) {
        if(other.getType() == BoundingVolumeType::AABB) {
            CollisionUtils::enclose(dynamic_cast<AABB&>(*this), dynamic_cast<const AABB&>(other));
        }
        else if(other.getType() == BoundingVolumeType::SPHERE) {
            CollisionUtils::enclose(dynamic_cast<AABB&>(*this), dynamic_cast<const BoundingSphere&>(other));
        }
    }
    else if(type == BoundingVolumeType::SPHERE) {
        if(other.getType() == BoundingVolumeType::AABB) {
            CollisionUtils::enclose(dynamic_cast<BoundingSphere&>(*this), dynamic_cast<const AABB&>(other));
        }
        else if(other.getType() == BoundingVolumeType::SPHERE) {
            CollisionUtils::enclose(dynamic_cast<BoundingSphere&>(*this), dynamic_cast<const BoundingSphere&>(other));
        }
    }
}

bool IBoundingVolume::intersects(const IBoundingVolume &other) const {
    if(type == BoundingVolumeType::AABB) {
        if(other.getType() == BoundingVolumeType::AABB) {
            return CollisionUtils::intersects(dynamic_cast<const AABB&>(*this), dynamic_cast<const AABB&>(other));
        }
        else if(other.getType() == BoundingVolumeType::SPHERE) {
            return CollisionUtils::intersects(dynamic_cast<const AABB&>(*this), dynamic_cast<const BoundingSphere&>(other));
        }
    }
    else if(type == BoundingVolumeType::SPHERE) {
        if(other.getType() == BoundingVolumeType::AABB) {
            return CollisionUtils::intersects(dynamic_cast<const AABB&>(other), dynamic_cast<const BoundingSphere&>(*this));
        }
        else if(other.getType() == BoundingVolumeType::SPHERE) {
            return CollisionUtils::intersects(dynamic_cast<const BoundingSphere&>(*this), dynamic_cast<const BoundingSphere&>(other));
        }
    }
    return false;
}