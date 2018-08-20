#pragma once

#include <string>

#include "Entity.h"


namespace Lag {
    class Material;

    //TODO: use a code (non XML) generated material (texture is the only input)
    class Sky : public Entity {
    public:
        explicit Sky(const std::string &materialName);
        //Sky(const std::string &textureName);
    };
}