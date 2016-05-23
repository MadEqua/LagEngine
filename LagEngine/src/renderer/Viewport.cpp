#include "Viewport.h"
#include "../scene/SceneManager.h"

using namespace Lag;

Viewport::Viewport(Camera &camera, RenderTarget &renderTarget, float left, float top, float width, float height) :
	camera(camera),
	renderTarget(renderTarget),
	left(left), top(top), width(width), height(height)
{
}

Viewport::~Viewport()
{
}

void Viewport::addRenderablesToQueue(RenderQueue &renderQueue, SceneManager &sceneManager)
{
	sceneManager.addRenderablesToQueue(renderQueue, camera);
}