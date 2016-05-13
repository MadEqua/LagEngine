#include "Viewport.h"

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

/*void Viewport::render(RenderQueue &renderQueue)
{

}*/