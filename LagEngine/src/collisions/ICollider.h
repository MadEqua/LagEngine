#pragma once

namespace Lag {

    class ICollider {
    public:
        virtual ~ICollider() = default;

        virtual void onCollision() = 0;
    };
}