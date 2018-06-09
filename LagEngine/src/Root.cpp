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

#include "io/Keys.h"

#ifdef ENABLE_DEBUG_MACRO
    #include "io/ResourceFilesWatcher.h"
#endif

using namespace Lag;

Root::Root() :
        inputManager(nullptr),
        renderer(nullptr),
        sceneManager(nullptr),
        renderTargetManager(nullptr),
        gpuProgramStageManager(nullptr),
        gpuProgramManager(nullptr),
        materialManager(nullptr),
        meshManager(nullptr),
        imageManager(nullptr),
        textureManager(nullptr),
        gpuBufferManager(nullptr),
        graphicsAPI(nullptr),
        inputDescriptionManager(nullptr),
        appResourcesFile(nullptr),
        lagResourcesFile(nullptr),
#ifdef ENABLE_DEBUG_MACRO
        resourceFilesWatcher(nullptr),
#endif
        initializationParameters(nullptr) {

    //Initialize other singletons
    LogManager::getInstance();
}

Root::~Root() {
    destroy();
}

void Root::destroy() {
    delete sceneManager;
    sceneManager = nullptr;

    delete materialManager;
    materialManager = nullptr;

    delete gpuProgramManager;
    gpuProgramManager = nullptr;

    delete gpuProgramStageManager;
    gpuProgramStageManager = nullptr;

    delete textureManager;
    textureManager = nullptr;

    delete imageManager;
    imageManager = nullptr;

    delete meshManager;
    meshManager = nullptr;

    delete gpuBufferManager;
    gpuBufferManager = nullptr;

    delete inputDescriptionManager;
    inputDescriptionManager = nullptr;

    delete graphicsAPI;
    graphicsAPI = nullptr;

    delete renderer;
    renderer = nullptr;

    delete inputManager;
    inputManager = nullptr;

    if(renderWindow.isValid())
        renderWindow.invalidate();

    delete renderTargetManager;
    renderTargetManager = nullptr;

#ifdef ENABLE_DEBUG_MACRO
    delete resourceFilesWatcher;
    resourceFilesWatcher = nullptr;
#endif

    delete initializationParameters;
    initializationParameters = nullptr;

    delete appResourcesFile;
    appResourcesFile = nullptr;

    delete lagResourcesFile;
    lagResourcesFile = nullptr;
}

bool Root::initializeLag(const std::string &iniFile) {
    //in case of reinitialization
    destroy();

    initializationParameters = new InitializationParameters(iniFile);

    IPlatformFactory *platformFactory = nullptr;
    if (initializationParameters->platformType == PlatformType::GLFW_GL4_5)
        platformFactory = new GLFW_GL4_5Factory();
    else {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "Root",
                                      "Cannot find a suitable platform factory. Double check the chosen platform on the .ini file.");
        return false;
    }

    bool result = internalInit(platformFactory);
    delete platformFactory;
    return result;
}

bool Root::internalInit(const IPlatformFactory *platformFactory) {
    renderTargetManager = new RenderTargetManager(platformFactory->getWindowRenderTargetBuilder(),
                                                  platformFactory->getTextureRenderTargetBuilder());
    renderWindow = renderTargetManager->getRenderWindow(*initializationParameters);
    if (!renderWindow.isValid())
        return false;

    inputManager = platformFactory->getInputManager(dynamic_cast<RenderWindow &>(*renderWindow));

    graphicsAPI = platformFactory->getGraphicsAPI();
    if (!graphicsAPI->initialize())
        return false;

    gpuBufferManager = new GpuBufferManager(platformFactory->getGpuBufferBuilder());
    gpuProgramManager = new GpuProgramManager(platformFactory->getGpuProgramBuilder());
    inputDescriptionManager = new InputDescriptionManager(platformFactory->getInputDescriptionBuilder());

    if (!initResources(platformFactory))
        return false;

    sceneManager = new SceneManager();
    renderer = new Renderer(*graphicsAPI, *sceneManager, *renderTargetManager);
    sceneManager->registerObservers(); //Needs to be called after Renderer creation

    dynamic_cast<RenderWindow *>(renderWindow.get())->registerObserver(windowListener);
    inputManager->registerObserver(keyboardListener);

#ifdef ENABLE_DEBUG_MACRO
    resourceFilesWatcher = new ResourceFilesWatcher(*initializationParameters);
#endif

    return true;
}

bool Root::initResources(const IPlatformFactory *platformFactory) {
    if (!initResourcesFiles())
        return false;

    textureManager = new TextureManager(platformFactory->getTextureBuilder(
            XmlResourceBuilderData(*appResourcesFile, *lagResourcesFile, "", "", TEXTURE_XML_TAG)));

    gpuProgramStageManager = new GpuProgramStageManager(platformFactory->getGpuProgramStageBuilder(
            XmlResourceBuilderData(*appResourcesFile, *lagResourcesFile,
                                   initializationParameters->getShadersFolder(false),
                                   initializationParameters->getShadersFolder(true),
                                   SHADER_XML_TAG)));

    imageManager = new ImageManager(new ImageBuilder(XmlResourceBuilderData(*appResourcesFile, *lagResourcesFile,
                                                                            initializationParameters->getImagesFolder(false),
                                                                            initializationParameters->getImagesFolder(true),
                                                                            IMAGE_XML_TAG)));

    meshManager = new MeshManager(new MeshBuilder(XmlResourceBuilderData(*appResourcesFile, *lagResourcesFile,
                                                                         initializationParameters->getMeshesFolder(false),
                                                                         initializationParameters->getMeshesFolder(true),
                                                                         MESH_XML_TAG)));

    materialManager = new MaterialManager(new MaterialBuilder(XmlResourceBuilderData(*appResourcesFile, *lagResourcesFile,
                                                       initializationParameters->getMaterialsFolder(false),
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

    auto *lagResourcesFile = new TiXmlDocument(lagResourcesFilePath);
    if (!checkResourcesFile(lagResourcesFilePath, lagResourcesFile)) {
        delete lagResourcesFile;
        return false;
    }

    auto *appResourcesFile = new TiXmlDocument(resourcesFilePath);
    if (!checkResourcesFile(resourcesFilePath, appResourcesFile)) {
        delete lagResourcesFile;
        delete appResourcesFile;
        return false;
    }

    this->appResourcesFile = appResourcesFile;
    this->lagResourcesFile = lagResourcesFile;
    return true;
}

bool Root::checkResourcesFile(const std::string &filePath, TiXmlDocument *resourcesFile) const {
    if (!resourcesFile->LoadFile()) {
        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL, "Root",
                                      "Resources file: " + filePath + " does not exist or is malformed.");
        return false;
    }

    const TiXmlElement *resourcesElement = resourcesFile->FirstChildElement();
    if (!resourcesElement) {
        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL, "Root",
                                      "Resources file: " + filePath + " does not contain <resources> element.");
        return false;
    }

    return true;
}

void Root::reloadResourcesFile() {
    auto oldAppResourcesFile = appResourcesFile;
    auto oldLagResourcesFile = lagResourcesFile;
    if (initResourcesFiles()) {
        delete oldAppResourcesFile;
        delete oldLagResourcesFile;
    }
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