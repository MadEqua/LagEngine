#include "RenderTarget.h"

#include "LogManager.h"

using namespace Lag;

RenderTarget::RenderTarget(uint32 width, uint32 height, RenderPhase renderPhase, bool isMainWindow) :
        width(width), height(height),
        mainWindow(isMainWindow),
        renderPhase(renderPhase) {
}

Viewport &RenderTarget::createViewport(Camera &camera, float left, float top, float width, float height) {
    auto *vp = new Viewport(static_cast<uint16>(viewports.getNextName()), camera, *this, left, top, width, height);
    viewports.add(vp);
    return *vp;
}

Viewport *RenderTarget::getViewport(uint16 name) const {
    return viewports.get(name);
}

void RenderTarget::clearViewports() {
    viewports.clear();
}

void RenderTarget::addRenderablesToQueue(RenderQueue &renderQueue, SceneManager &sceneManager) {
    if (viewports.isEmpty()) {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "RenderTarget",
                                      "RenderTarget " + name + " does not contain Viewports. At least one is needed.");
        return;
    }

    onPreRenderNotify(*this);
    for (uint32 i = 0; i < viewports.getSize(); ++i) {
        Viewport *v = viewports.get(i);
        v->addRenderablesToQueue(renderQueue, sceneManager, *this);
    }
}

void RenderTarget::resize(uint32 width, uint32 height) {
    this->width = width;
    this->height = height;
    onResizeNotify(*this, width, height);
}