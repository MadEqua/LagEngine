#include "Renderer.h"

#include "RenderTarget.h"

#include "../graphicsAPIs/gl4/GL4GraphicsAPI.h"
#include "graphicsApi/IGraphicsApi.h"

#include "../scene/SceneManager.h"
#include "../scene/Entity.h"

#include "Material.h"
#include "Technique.h"
#include "Pass.h"

#include "../io/log/LogManager.h"

using namespace Lag;

Renderer::Renderer(SceneManager &sceneManager) :
	sceneManager(sceneManager)
{
}

Renderer::~Renderer()
{
}

bool Renderer::initialize(const GraphicsApiType graphicsApiType)
{
	switch (graphicsApiType)
	{
	case OPENGL_4:
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
	//TODO: rethink this chain of calls...
	/*for (auto &pair : renderTargets)
		pair.second->startRender(renderQueue);*/


	renderQueue.clear();
	const std::vector<Entity*> &objects = sceneManager.getEntities();
	for (Entity* entity : objects)
	{
		Material &material = entity->getMaterial();
		//Technique &technique = material.getTechnique();
		//Pass &pass = technique.getPass();

		ShaderProgram &shaderProgram = material.getShaderProgram();
		Renderable &renderableToFill = renderQueue.getNextSlotToFill();

		//fill renderable
	}


	//pass through all scene objects
	//find the correct technique from the material
	//extract passes and create a renderable with all the required data
	//add renderable to queue
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