#include "Viewport.h"
#include "../scene/SceneManager.h"
#include "../scene/Camera.h"
#include "RenderTarget.h"
#include <glm/glm.hpp>

using namespace Lag;

Viewport::Viewport(const std::string &name, Camera &camera, RenderTarget &renderTarget,
	float left, float bottom, float width, float height) :
	name(name),
	camera(camera),
	renderTarget(renderTarget),
	left(left), bottom(bottom), width(width), height(height),
	renderTargetListener(*this)

{
	computeCameraAspectRatio();
	renderTarget.registerObserver(renderTargetListener);
}

Viewport::~Viewport()
{
}

void Viewport::addRenderablesToQueue(RenderQueue &renderQueue, SceneManager &sceneManager)
{
	sceneManager.addRenderablesToQueue(renderQueue, *this);
}

uint32 Viewport::getRealLeft() const
{
	return static_cast<uint32>(static_cast<float>(renderTarget.getWidth()) * left);
}

uint32 Viewport::getRealBottom() const
{
	return static_cast<uint32>(static_cast<float>(renderTarget.getHeight()) * bottom);
}

uint32 Viewport::getRealWidth() const
{
	return static_cast<uint32>(glm::clamp(static_cast<float>(renderTarget.getWidth()) * width,
		0.0f, static_cast<float>(renderTarget.getWidth())));
}

uint32 Viewport::getRealHeight() const
{
	return static_cast<uint32>(glm::clamp(static_cast<float>(renderTarget.getHeight()) * height,
		0.0f, static_cast<float>(renderTarget.getHeight())));
}

void Viewport::computeCameraAspectRatio() const
{
	camera.setAspectRatio(static_cast<float>(getRealWidth()) / static_cast<float>(getRealHeight()));
}

Viewport::RenderTargetListener::RenderTargetListener(const Viewport &vp) :
	viewport(vp)
{
}

void Viewport::RenderTargetListener::onPreRender(RenderTarget &notifier)
{
}

void Viewport::RenderTargetListener::onPostRender(RenderTarget &notifier)
{
}

void Viewport::RenderTargetListener::onResize(RenderTarget &notifier, int width, int height)
{
	viewport.computeCameraAspectRatio();
}