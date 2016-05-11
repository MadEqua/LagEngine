#include "Renderer.h"

#include "RenderTarget.h"
#include "RenderWindowParameters.h"
#include "graphicsApi/IGraphicsApi.h"
#include "../graphicsAPIs/gl4/GL4GraphicsAPI.h"
#include "../io/log/LogManager.h"


using namespace Lag;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::initialize(const GpuInterface &gpuInterface)
{
	switch (gpuInterface)
	{
	case OPENGL4:
		this->graphicsAPI = new GL4GraphicsAPI();
		return true;
	default:
		return false;
	}

	LogManager::getInstance().log(FILE, NORMAL, INFO, "Renderer", "Initialized successfully.");
}

void Renderer::shutdown()
{
	LogManager::getInstance().log(FILE, NORMAL, INFO, "Renderer", "Destroyed successfully.");
}

void Renderer::addRenderTarget(const std::string &name, RenderTarget &renderTarget)
{
	renderTargets[name] = &renderTarget;
}

void Renderer::removeRenderTarget(const std::string &name)
{
	renderTargets.erase(name);
}

void Renderer::renderAllRenderTargets()
{
	for (auto &pair : renderTargets)
		pair.second->startRender();
}

/* Renderer::setViewport(const Viewport &vp)
{

}*/

/*void Renderer::setWorldMatrix()
{

}

void Renderer::setViewMatrix()
{
}

void Renderer::setProjectionMatrix()
{

}*/

void Renderer::clearFrameBuffer(/*buffers, color?*/)
{

}

void Renderer::render(const Renderable &renderable)
{

}