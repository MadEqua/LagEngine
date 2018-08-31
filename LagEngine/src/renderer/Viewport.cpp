#include "Viewport.h"
#include "SceneManager.h"
#include "Camera.h"
#include "RenderTarget.h"

using namespace Lag;

Viewport::Viewport(uint16 name, Camera &camera, RenderTarget &renderTarget,
                   float left, float bottom, float width, float height) :
        name(name),
        camera(camera),
        renderTarget(renderTarget),
        left(left), bottom(bottom), width(width), height(height) {
    computeCameraAspectRatio();
    renderTarget.registerObserver(*this);
}

Viewport::~Viewport() {
    renderTarget.unregisterObserver(*this);
}

void Viewport::addRenderablesToQueue(RenderQueue &renderQueue, SceneManager &sceneManager, RenderTarget &renderTarget) {
    sceneManager.addRenderablesToQueue(renderQueue, *this, renderTarget);
}

uint32 Viewport::getRealLeft() const {
    return static_cast<uint32>(static_cast<float>(renderTarget.getWidth()) * left);
}

uint32 Viewport::getRealBottom() const {
    return static_cast<uint32>(static_cast<float>(renderTarget.getHeight()) * bottom);
}

uint32 Viewport::getRealWidth() const {
    return static_cast<uint32>(glm::clamp(static_cast<float>(renderTarget.getWidth()) * width,
                                          0.0f, static_cast<float>(renderTarget.getWidth())));
}

uint32 Viewport::getRealHeight() const {
    return static_cast<uint32>(glm::clamp(static_cast<float>(renderTarget.getHeight()) * height,
                                          0.0f, static_cast<float>(renderTarget.getHeight())));
}

glm::u32vec2 Viewport::getSize() const {
    return glm::u32vec2(getRealWidth(), getRealHeight());
}

void Viewport::computeCameraAspectRatio() const {
    camera.setAspectRatio(static_cast<float>(getRealWidth()) / static_cast<float>(getRealHeight()));
}

void Viewport::onPreRender(RenderTarget &notifier) {
}

void Viewport::onPostRender(RenderTarget &notifier) {
}

void Viewport::onResize(RenderTarget &notifier, uint32 width, uint32 height) {
    computeCameraAspectRatio();
}