#pragma once

#include "SceneObject.h"
#include "Color.h"

namespace Lag {
    class Camera;

    class Light : public SceneObject {
    public:
        Light(uint32 name, const Color &color, bool castShadow = true);

        inline const Color &getColor() const { return color; }

        inline bool getCastShadow() const { return castShadow; }

    protected:
        Color color;

        bool castShadow;

        //For shadow mapping, if castShadow == true
        Camera *camera;
    };
}