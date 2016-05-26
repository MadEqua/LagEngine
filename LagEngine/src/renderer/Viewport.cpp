#include "Viewport.h"
#include "../scene/SceneManager.h"
#include "../scene/Camera.h"
#include "RenderTarget.h"
#include <glm/glm.hpp>

using namespace Lag;

Viewport::Viewport(Camera &camera, RenderTarget &renderTarget, float left, float top, float width, float height) :
	camera(camera),
	renderTarget(renderTarget),
	left(left), top(top), width(width), height(height),
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

uint32 Viewport::getRealTop() const
{
	return static_cast<uint32>(static_cast<float>(renderTarget.getHeight()) * top);
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

void Viewport::RenderTargetListener::onPreRender()
{
}

void Viewport::RenderTargetListener::onPostRender()
{
}

void Viewport::RenderTargetListener::onResize(int width, int height)
{
	viewport.computeCameraAspectRatio();
}