#include "DirectionalLight.h"

#include "../renderer/Renderer.h"
#include "../renderer/RenderTarget.h"
#include "../scene/OrthographicCamera.h"
#include "../scene/SceneNode.h"
#include "SceneManager.h"
#include "../Root.h"

using namespace Lag;

DirectionalLight::DirectionalLight(uint32 name, const glm::vec3 &direction, const Color &color, bool castShadow) :
	 Light(name, color, castShadow), direction(direction)
{
	if (castShadow)
	{
		Root& root = Root::getInstance();

		RenderTarget &rt = *root.getRenderer().getRenderTarget(0);
		//TODO...
		//RenderTarget &rt = root.getRenderer().createRenderTarget("", 1280, 800); //TODO: size and resizes?
		const float S = 30;
		Camera &camera = root.getSceneManager().createOrthographicCamera(-S, S, -S, S, 1, 100); //TODO: auto size
		SceneNode &sn = root.getSceneManager().getSceneGraph().getRootSceneNode().createChildSceneNode("");
			
		glm::vec3 dir = -direction;
		glm::vec3 pos = 10.0f * dir;
		sn.lookAt(pos, glm::vec3(0), glm::vec3(0, 1, 0));

		camera.attachToSceneNode(sn);
		rt.createViewport(camera);
	}
}

DirectionalLight::~DirectionalLight()
{
}