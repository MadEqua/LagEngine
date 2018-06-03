#include "TestScene2.h"

#include "Root.h"
#include "renderer/RenderWindow.h"
#include "renderer/Renderer.h"
#include "utils/FreeCamera.h"
#include "scene/SceneNode.h"
#include "scene/Entity.h"
#include "renderer/Color.h"

void TestScene2::onStart()
{
	Lag::Root &root = Lag::Root::getInstance();

	root.getRenderer().setPointSizeFromGpuProgramEnabled(true);

	Lag::SceneNode &mainNode = getSceneGraph().getRootSceneNode().createChildSceneNode("main");
	Lag::Entity *ent = createEntity("piano", "objectMaterial");
	ent->setRenderMode(Lag::RenderMode::LINE_POINTS);
	ent->attachToSceneNode(mainNode);
}

void TestScene2::onEnd()
{
	delete camera;
}

void TestScene2::onInitializeViewports(Lag::RenderWindow &renderWindow)
{
	camera = new Lag::FreeCamera(*this, 45.0f, 0.1f, 1000.0f, 10.0f);
	camera->getCamera().getParentSceneNode()->setPosition(glm::vec3(0, 10, 20));

	renderWindow.createViewport(camera->getCamera());
}