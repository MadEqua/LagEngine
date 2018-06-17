#include "RenderTargetManager.h"

#include "RenderWindow.h"

using namespace Lag;

RenderTargetBuilder::RenderTargetBuilder() :
        width(400), height(400),
        renderPhase(RenderPhase::COLOR) {
}

void RenderTargetBuilder::setBuildRenderWindow(const InitializationParameters &parameters) {
    initializationParameters = &parameters;
}

void RenderTargetBuilder::setBuildRenderToTexture(uint32 width, uint32 height, RenderPhase renderPhase) {
    this->width = width;
    this->height = height;
    this->renderPhase = renderPhase;
}

RenderTargetManager::RenderTargetManager(RenderTargetBuilder *renderWindowBuilder,
                                         RenderTargetBuilder *renderTextureBuilder) :
        Manager("RenderTargetManager", renderWindowBuilder),
        renderWindowBuilder(renderWindowBuilder), renderTextureBuilder(renderTextureBuilder),
        nextName(1) {
}

RenderTargetManager::~RenderTargetManager() {
    if (builder != renderTextureBuilder)
        delete renderTextureBuilder;
    if (builder != renderWindowBuilder)
        delete renderWindowBuilder;
}

Handle<RenderTarget> RenderTargetManager::getRenderWindow(const InitializationParameters &parameters) {
    renderWindowBuilder->setBuildRenderWindow(parameters);
    builder = renderWindowBuilder;
    return Manager::get(0);
}

RenderWindow *RenderTargetManager::getRenderWindow() {
    return dynamic_cast<RenderWindow *>(RenderTargetManager::get(0).get());
}

Handle<RenderTarget> RenderTargetManager::getRenderToTexture(uint32 width, uint32 height, RenderPhase renderPhase) {
    renderTextureBuilder->setBuildRenderToTexture(width, height, renderPhase);
    builder = renderTextureBuilder;
    return Manager::get(getNextName());
}

void RenderTargetManager::resetToBasicState() {
    for (auto it = objects.begin(); it != objects.end();) {
        if (it->second->isMainWindow()) {
            it->second->clearViewports();
            ++it;
        }
        else
            it = deleteEntry(it);
    }
}

uint32 RenderTargetManager::getNextName() {
    return nextName++;
}