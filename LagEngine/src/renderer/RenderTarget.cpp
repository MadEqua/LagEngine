#include "RenderTarget.h"

#include "../io/log/LogManager.h"
#include "Viewport.h"

using namespace Lag;

RenderTarget::RenderTarget(uint32 width, uint32 height, RenderPhase renderPhase, bool isMainWindow) :
	width(width), height(height), 
	mainWindow(isMainWindow),
	renderPhase(renderPhase)
{
}

Viewport& RenderTarget::createViewport(Camera &camera, float left, float top, float width, float height)
{
	Viewport *vp = new Viewport(viewports.getNextName(), camera, *this, left, top, width, height);
	viewports.add(vp);
	return *vp;
}

Viewport* RenderTarget::getViewport(uint16 name) const
{
	return viewports.get(name);
}

void RenderTarget::clearViewports()
{
	viewports.clear();
}

void RenderTarget::addRenderablesToQueue(RenderQueue &renderQueue, SceneManager &sceneManager)
{
	if (viewports.isEmpty())
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL,
			"RenderTarget", "RenderTarget " + name + " does not contain Viewports. At least one is needed.");
		return;
	}
	
	onPreRenderNotify(*this);
	for (uint32 i = 0; i < viewports.getSize(); ++i)
	{
		//TODO: add proper iterator
		Viewport *v = viewports.get(i);
		v->addRenderablesToQueue(renderQueue, sceneManager, *this);
	}
	onPostRenderNotify(*this); //TODO fix. this is not post render.
}

void RenderTarget::resize(uint32 width, uint32 height)
{
	this->width = width;
	this->height = height;
	onResizeNotify(*this, width, height);
}

LAG_DEFINE_NOTIFY_METHOD(RenderTarget, onPreRender, IRenderTargetListener, LAG_ARGS(RenderTarget &notifier), LAG_ARGS(notifier))
LAG_DEFINE_NOTIFY_METHOD(RenderTarget, onPostRender, IRenderTargetListener, LAG_ARGS(RenderTarget &notifier), LAG_ARGS(notifier))
LAG_DEFINE_NOTIFY_METHOD(RenderTarget, onResize, IRenderTargetListener, LAG_ARGS(RenderTarget &notifier, uint32 width, uint32 height), LAG_ARGS(notifier, width, height))