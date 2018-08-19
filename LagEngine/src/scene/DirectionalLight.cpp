#include "DirectionalLight.h"

#include "SceneNode.h"
#include "SceneManager.h"

using namespace Lag;

DirectionalLight::DirectionalLight(const glm::vec3 &direction, const Color &color, bool castShadow) :
        Light(color, castShadow),
        direction(direction) {
    /*if (castShadow)
    {
        Root& root = Root::getInstance();
        RenderToTexture *rt = root.getRenderTargetManager().getRenderToTexture(1280, 800, RenderPhase::DEPTH); //TODO: size and resizes?

        ImageData imageData;
        imageData.width = 1280;
        imageData.height = 800;
        imageData.components= ImageComponents::R;
        imageData.componentType = ImageComponentType::FLOAT32;

        TextureData textureData;
        textureData.type = TextureType::TYPE_2D;
        textureData.dataType = TextureDataType::DEPTH;
        textureData.mipmaps = 1;
        textureData.magnificationFilteringMode = TextureFilteringMode::NEAREST;
        textureData.minificationFilteringMode = TextureFilteringMode::NEAREST;

        rt->attachDepthTexture(imageData, textureData);

        if (!rt->checkCompleteness())
            LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "DirectionalLight", "RTT completeness check failed.");

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