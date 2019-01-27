#pragma once

#include "Entity.h"
#include "Color.h"

class Plane : public Lag::Entity {
public:
    Plane(const Lag::Color &color1, const Lag::Color &color2, float trisPerLength, float maxPointSize, float displacementStrength);

protected:
    void onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) override;

private:
    Lag::uint32 color1, color2;
    float trisPerLength; //Control tesselation
    float maxPointSize;
    float displacementStrength;
};