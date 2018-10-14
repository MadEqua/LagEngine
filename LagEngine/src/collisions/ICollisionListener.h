#pragma once

#include <string>

namespace Lag {

    class Entity;

    class ICollisionListener {
    public:
        virtual ~ICollisionListener() = default;

        virtual void onCollision(Entity &entity1, Entity &entity2) = 0;
    };
}