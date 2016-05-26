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

void RenderTarget::addRenderablesToQueue(RenderQueue &renderQueue, SceneManager &sceneManager)
{
	onPreRenderNotify();
	for (auto &pair : viewports)
		pair.second->addRenderablesToQueue(renderQueue, sceneManager);
	onPostRenderNotify();
}

void RenderTarget::resize(int width, int height)
{
	this->width = width;
	this->height = height;
	onResizeNotify(width, height);
}

LAG_DEFINE_NOTIFY_METHOD(RenderTarget, onPreRender, IRenderTargetListener, LAG_ARGS(), LAG_ARGS())
LAG_DEFINE_NOTIFY_METHOD(RenderTarget, onPostRender, IRenderTargetListener, LAG_ARGS(), LAG_ARGS())
LAG_DEFINE_NOTIFY_METHOD(RenderTarget, onResize, IRenderTargetListener, LAG_ARGS(int width, int height), LAG_ARGS(width, height))