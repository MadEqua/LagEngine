#include "DirectionalLight.h"

#include "../renderer/RenderTargetManager.h"
#include "../renderer/graphicsAPI/RenderToTexture.h"
#include "../scene/OrthographicCamera.h"
#include "../scene/SceneNode.h"
#include "SceneManager.h"
#include "../resources/Image.h"
#include "../renderer/graphicsAPI/Texture.h"
#include "../Root.h"
#include "../io/log/LogManager.h"

using namespace Lag;

DirectionalLight::DirectionalLight(uint32 name, const glm::vec3 &direction, const Color &color, bool castShadow) :
	Light(name, color, castShadow),
	direction(direction)
{
	/*if (castShadow)
	{
		Root& root = Root::getInstance();
		RenderToTexture *rt = root.getRenderTargetManager().getRenderToTexture(1280, 800, LAG_RENDER_PHASE_DEPTH); //TODO: size and resizes?
		
		ImageData imageData; 
		imageData.width = 1280;
		imageData.height = 800;
		imageData.components= LAG_IMAGE_COMPONENTS_R;
		imageData.componentType = LAG_IMAGE_COMPONENT_TYPE_FLOAT32;

		TextureData textureData;
		textureData.type = LAG_TEXTURE_TYPE_2D;
		textureData.dataType = LAG_TEXTURE_DATA_TYPE_DEPTH;
		textureData.mipmaps = 1;
		textureData.magnificationFilteringMode = LAG_TEXTURE_FILTERING_MODE_NEAREST;
		textureData.minificationFilteringMode = LAG_TEXTURE_FILTERING_MODE_NEAREST;

		rt->attachDepthTexture(imageData, textureData);

		if (!rt->checkCompleteness())
			LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "DirectionalLight", "RTT completeness check failed.");

		const float S = 30;
		Camera &camera = root.getSceneManager().getCurrentScene().createOrthographicCamera(-S, S, -S, S, 1, 100); //TODO: auto size and auto move with light
		SceneNode &sn = root.getSceneManager().getCurrentScene().getSceneGraph().getRootSceneNode().createChildSceneNode("");
			
		glm::vec3 dir = -direction;
		glm::vec3 pos = 10.0f * dir;
		sn.lookAt(pos, glm::vec3(0), glm::vec3(0, 1, 0));

		camera.attachToSceneNode(sn);
		rt->createViewport(camera);
	}*/
}