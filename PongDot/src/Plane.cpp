#include "Plane.h"

#include "GpuProgram.h"
#include "GpuProgramUniform.h"
#include "Handle.h"
#include "Material.h"
#include "Color.h"


Plane::Plane(const Lag::Color &color1, const Lag::Color &color2) : 
    Entity("plane", "pointMaterial") {

    this->color1 = color1.toIntABGR();
    this->color2 = color2.toIntABGR();
}

void Plane::onSubEntityPreRender(Lag::SubEntity &subEntity, Lag::Renderer &renderer, const Lag::RenderOperation &renderOperation) {
    Entity::onSubEntityPreRender(subEntity, renderer, renderOperation);

    material->getGpuProgram().getUniformByName("color1")->setValue(reinterpret_cast<const void*>(&color1));
    material->getGpuProgram().getUniformByName("color2")->setValue(reinterpret_cast<const void*>(&color2));
}