#include "RenderTarget.h"
#include "Viewport.h"

using namespace Lag;

RenderTarget::RenderTarget(uint32 width, uint32 height) :
	width(width), height(height)
{
}

RenderTarget::~RenderTarget()
{
	for (auto &pair : viewports)
		delete pair.second;
}

Viewport& RenderTarget::createViewport(const std::string &name, Camera &camera, float left, float top, float width, float height)
{
	Viewport *vp = new Viewport(camera, *this, left, top, width, height);
	viewports[name] = vp;
	return *vp;
}

Viewport* RenderTarget::getViewport(const std::string &name) const
{
	auto it = viewports.find(name);
	if (it != viewports.end())
		return it->second;
	else
		return nullptr;
}

/*void RenderTarget::startRender(RenderQueue &renderQueue)
{
	onPreRenderNotify();
	for (auto &pair : viewports)
		pair.second->render(renderQueue);
	onPostRenderNotify();
}*/

DEFINE_NOTIFY_METHOD(RenderTarget, onPreRender, IRenderTargetListener, ARGS(), ARGS())
DEFINE_NOTIFY_METHOD(RenderTarget, onPostRender, IRenderTargetListener, ARGS(), ARGS())