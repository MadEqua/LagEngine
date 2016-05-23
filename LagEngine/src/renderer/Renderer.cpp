#include "Renderer.h"

#include "RenderTarget.h"
#include "../scene/SceneManager.h"
#include "../io/log/LogManager.h"

using namespace Lag;

Renderer::Renderer(IGraphicsAPI &graphicsAPI, SceneManager &sceneManager) :
	sceneManager(sceneManager), graphicsAPI(graphicsAPI)
{
	LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
		"Renderer", "Initialized successfully.");
}

Renderer::~Renderer()
{
	LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
		"Renderer", "Destroyed successfully.");
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
	renderQueue.clear();
	
	for (auto &rtPair : renderTargets)
	{
		RenderTarget &rt = *rtPair.second;
		rt.addRenderablesToQueue(renderQueue, sceneManager);
	}

	renderQueue.sort();
	renderQueue.dispatchRenderOperations(graphicsAPI);
}