#pragma once

#include "SceneObject.h"
#include "Color.h"

namespace Lag {
    class Camera;

    class Light : public SceneObject {
    public:
        Light(const Color &color, bool castShadow = true);

        inline const Color &getColor() const { return color; }
        inline void setColor(const Color &color) { this->color = color; }
        inline bool getCastShadow() const { return castShadow; }

    protected:
        Color color;

        bool castShadow;

        //For shadow mapping, if castShadow == true
        Camera *camera;
    };
}