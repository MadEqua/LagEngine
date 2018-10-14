#pragma once

#include "ObserverPattern.h"
#include "ICollisionListener.h"

namespace Lag {

    class Entity;

    class CollisionManager {

        LAG_GENERATE_OBSERVER_STORAGE(ICollisionListener)
        LAG_GENERATE_NOTIFY_METHOD(onCollision, ICollisionListener, LAG_ARGS(Entity &entity1, Entity &entity2), LAG_ARGS(entity1, entity2))

    public:
        void checkCollisions() const;
    };
};