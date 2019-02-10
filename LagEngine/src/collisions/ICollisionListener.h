#pragma once

#include <string>

#include "IntersectionResult.h"

namespace Lag {

    class Entity;

    class ICollisionListener {
    public:
        virtual ~ICollisionListener() = default;

        virtual void onCollision(Entity &entity1, Entity &entity2, const IntersectionResult &result) = 0;
    };
}