#include "RenderTarget.h"
#include "Viewport.h"

using namespace Lag;

RenderTarget::RenderTarget(uint32 width, uint32 height, bool isMainWindow) :
	width(width), height(height), 
	mainWindow(isMainWindow)
{
}

RenderTarget::~RenderTarget()
{
}

Viewport& RenderTarget::createViewport(Camera &camera, 
	float left, float top, float width, float height)
{
	Viewport *vp = new Viewport(viewports.getNextName(), camera, *this, left, top, width, height);
	viewports.add(vp);
	return *vp;
}

Viewport* RenderTarget::getViewport(uint16 name) const
{
	return viewports.get(name);
}

void RenderTarget::addRenderablesToQueue(RenderQueue &renderQueue, SceneManager &sceneManager)
{
	onPreRenderNotify(*this);
	for (uint32 i = 0; i < viewports.getSize(); ++i)
	{
		//TODO: add proper iterator
		Viewport *v = viewports.get(i);
		if (v != nullptr)
			v->addRenderablesToQueue(renderQueue, sceneManager);
	}
	onPostRenderNotify(*this); //TODO wrong
}

void RenderTarget::resize(int width, int height)
{
	this->width = width;
	this->height = height;
	onResizeNotify(*this, width, height);
}

LAG_DEFINE_NOTIFY_METHOD(RenderTarget, onPreRender, IRenderTargetListener, LAG_ARGS(RenderTarget &notifier), LAG_ARGS(notifier))
LAG_DEFINE_NOTIFY_METHOD(RenderTarget, onPostRender, IRenderTargetListener, LAG_ARGS(RenderTarget &notifier), LAG_ARGS(notifier))
LAG_DEFINE_NOTIFY_METHOD(RenderTarget, onResize, IRenderTargetListener, LAG_ARGS(RenderTarget &notifier, int width, int height), LAG_ARGS(notifier, width, height))