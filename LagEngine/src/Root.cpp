#include "Root.h"
#include "tinyxml/tinyxml.h"

#include "GLFW_GL4_5Factory.h"

#include "LogManager.h"

#include "Renderer.h"
#include "RenderWindow.h"
#include "IGraphicsAPI.h"

#include "InputManager.h"
#include "SceneManager.h"

#include "GpuProgramManager.h"
#include "InputDescriptionManager.h"
#include "GpuBufferManager.h"
#include "MaterialManager.h"
#include "MeshManager.h"
#include "ImageManager.h"
#include "TextureManager.h"
#include "GpuProgramStageManager.h"
#include "MeshRepository.h"

#include "io/Keys.h"

#ifdef ENABLE_DEBUG_MACRO
    #include "io/ResourceFilesWatcher.h"
#endif


using namespace Lag;

Root::Root() : initialized(false) {

    //Initialize other singletons
    LogManager::getInstance();
}

Root::~Root() = default;

bool Root::initializeLag(const std::string &iniFile) {
    if(initialized) return true;

    initializationParameters = std::make_unique<InitializationParameters>(iniFile);

    std::unique_ptr<IPlatformFactory> platformFactory;
    if (initializationParameters->platformType == PlatformType::GLFW_GL4_5)
        platformFactory = std::make_unique<GLFW_GL4_5Factory>();
    else {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "Root",
                                      "Cannot find a suitable platform factory. Double check the chosen platform on the .ini file.");
        return false;
    }

    bool result = internalInit(*platformFactory);
    initialized = result;
    return result;
}

bool Root::internalInit(const IPlatformFactory &platformFactory) {
    renderTargetManager = std::make_unique<RenderTargetManager>(platformFactory.getWindowRenderTargetBuilder(), platformFactory.getTextureRenderTargetBuilder());
    renderWindow = renderTargetManager->getRenderWindow(*initializationParameters);
    if (!renderWindow.isValid())
        return false;

    inputManager = std::unique_ptr<InputManager>(platformFactory.getInputManager(dynamic_cast<RenderWindow &>(*renderWindow)));

    graphicsAPI = std::unique_ptr<IGraphicsAPI>(platformFactory.getGraphicsAPI());
    if (!graphicsAPI->initialize())
        return false;

    gpuBufferManager = std::make_unique<GpuBufferManager>(platformFactory.getGpuBufferBuilder());
    gpuProgramManager = std::make_unique<GpuProgramManager>(platformFactory.getGpuProgramBuilder());
    inputDescriptionManager = std::make_unique<InputDescriptionManager>(platformFactory.getInputDescriptionBuilder());

    if (!initResources(platformFactory))
        return false;

    sceneManager = std::make_unique<SceneManager>();
    renderer = std::make_unique<Renderer>(*graphicsAPI, *sceneManager, *renderTargetManager);
    sceneManager->registerObservers(); //Needs to be called after Renderer creation

    dynamic_cast<RenderWindow *>(renderWindow.get())->registerObserver(windowListener);
    inputManager->registerObserver(keyboardListener);

#ifdef ENABLE_DEBUG_MACRO
    resourceFilesWatcher = std::make_unique<ResourceFilesWatcher>(*initializationParameters);
#endif

    meshRepository = std::make_unique<MeshRepository>(*meshManager);

    return true;
}

bool Root::initResources(const IPlatformFactory &platformFactory) {
    if (!initResourcesFiles())
        return false;

    textureManager = std::make_unique<TextureManager>(platformFactory.getTextureBuilder(XmlResourceBuilderData("", "", TEXTURE_XML_TAG)));

    gpuProgramStageManager = std::make_unique<GpuProgramStageManager>(platformFactory.getGpuProgramStageBuilder(
            XmlResourceBuilderData(initializationParameters->getShadersFolder(false),
                                   initializationParameters->getShadersFolder(true),
                                   SHADER_XML_TAG)));

    imageManager = std::make_unique<ImageManager>(new ImageBuilder(XmlResourceBuilderData(initializationParameters->getImagesFolder(false),
                                                                            initializationParameters->getImagesFolder(true),
                                                                            IMAGE_XML_TAG)));

    meshManager = std::make_unique<MeshManager>(new MeshBuilder(XmlResourceBuilderData(initializationParameters->getMeshesFolder(false),
                                                                         initializationParameters->getMeshesFolder(true),
                                                                         MESH_XML_TAG)));

    materialManager = std::make_unique<MaterialManager>(new MaterialBuilder(XmlResourceBuilderData(initializationParameters->getMaterialsFolder(false),
                                                       initializationParameters->getMaterialsFolder(true),
                                                       MATERIAL_XML_TAG)));

    imageManager->initializeFallbackObject();
    gpuProgramManager->initializeFallbackObject();
    textureManager->initializeFallbackObject();
    materialManager->initializeFallbackObject();
    meshManager->initializeFallbackObject();

    return true;
}

bool Root::initResourcesFiles() {
    std::string lagResourcesFilePath = initializationParameters->lagResourcesFolder + '/' + initializationParameters->resourcesFile;
    std::string resourcesFilePath = initializationParameters->appResourcesFolder + '/' + initializationParameters->resourcesFile;

    auto lagResourcesFile = std::make_unique<TiXmlDocument>(lagResourcesFilePath);
    if (!checkResourcesFile(lagResourcesFilePath, *lagResourcesFile)) {
        return false;
    }

    auto appResourcesFile = std::make_unique<TiXmlDocument>(resourcesFilePath);
    if (!checkResourcesFile(resourcesFilePath, *appResourcesFile)) {
        return false;
    }

    this->appResourcesFile = std::move(appResourcesFile);
    this->lagResourcesFile = std::move(lagResourcesFile);
    return true;
}

bool Root::checkResourcesFile(const std::string &filePath, TiXmlDocument &resourcesFile) const {
    if (!resourcesFile.LoadFile()) {
        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL, "Root",
                                      "Resources file: " + filePath + " does not exist or is malformed.");
        return false;
    }

    const TiXmlElement *resourcesElement = resourcesFile.FirstChildElement();
    if (!resourcesElement) {
        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL, "Root",
                                      "Resources file: " + filePath + " does not contain <resources> element.");
        return false;
    }

    return true;
}

void Root::reloadResourcesFile() {
    initResourcesFiles();
}

void Root::startRenderingLoop() {
    renderer->startRenderingLoop(initializationParameters->maxFPS);
}

void Root::stopRenderingLoop() {
    renderer->stopRenderingLoop();
}

void Root::renderOneFrame() {
    renderer->renderOneFrame();
}

void Root::KeyboardListener::onKeyPress(int key, int modifier) {
    switch (key) {
        case KEY_LEFT_CONTROL: {
            RenderWindow *renderWindow = Root::getInstance().getRenderTargetManager().getRenderWindow();
            renderWindow->setVirtualCursor(!renderWindow->isVirtualCursorEnabled());
        }
            break;
        case KEY_ESCAPE:
            Root::getInstance().stopRenderingLoop();
            break;
        default:
            break;
    }
}

void Root::WindowListener::onClose(RenderWindow &notifier) {
    Root::getInstance().stopRenderingLoop();
}