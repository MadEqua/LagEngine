#include "RenderTargetManager.h"

#include "RenderWindow.h"
#include "graphicsAPI/RenderToTexture.h"

using namespace Lag;

RenderTargetBuilder::RenderTargetBuilder() :
	width(400), height(400),
	renderPhase(LAG_RENDER_PHASE_COLOR)
{
}

void RenderTargetBuilder::setBuildRenderWindow(const InitializationParameters &parameters)
{
	initializationParameters = parameters;
}

void RenderTargetBuilder::setBuildRenderToTexture(uint32 width, uint32 height, RenderPhase renderPhase)
{
	this->width = width;
	this->height = height;
	this->renderPhase = renderPhase;
}

RenderTargetManager::RenderTargetManager(RenderTargetBuilder* renderWindowBuilder, RenderTargetBuilder* renderTextureBuilder) :
	Manager("RenderTargetManager", renderWindowBuilder),
	renderWindowBuilder(renderWindowBuilder), renderTextureBuilder(renderTextureBuilder),
	nextName(1)
{
}

RenderTargetManager::~RenderTargetManager()
{
	if (builder != renderTextureBuilder)
		delete renderTextureBuilder;
	if (builder != renderWindowBuilder)
		delete renderWindowBuilder;
}

Handle<RenderTarget> RenderTargetManager::getRenderWindow(const InitializationParameters &parameters)
{
	renderWindowBuilder->setBuildRenderWindow(parameters);
	builder = renderWindowBuilder;
	return Manager::get(0);
}

Handle<RenderTarget> RenderTargetManager::getRenderWindow()
{
	builder = renderWindowBuilder;
	return Manager::get(0);
}

Handle<RenderTarget> RenderTargetManager::getRenderToTexture(uint32 width, uint32 height, RenderPhase renderPhase)
{
	renderTextureBuilder->setBuildRenderToTexture(width, height, renderPhase);
	builder = renderTextureBuilder;
	return Manager::get(getNextName());
}

uint32 RenderTargetManager::getNextName()
{
	return nextName++;
}