#include "ResourceFilesWatcher.h"

#include "filewatch/filewatch.h"

#include "Root.h"

#include "MaterialManager.h"
#include "ImageManager.h"
#include "TextureManager.h"
#include "GpuProgramStageManager.h"
#include "GpuProgramManager.h"
#include "Renderer.h"

using namespace Lag;

ResourceFilesWatcher::ResourceFilesWatcher(const InitializationParameters &initializationParameters) :
        fileWatcher(nullptr) {
    LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL, "ResourceFilesWatcher",
                                  "ResourceFileWatcher enabled.");

    Root::getInstance().getRenderer().registerObserver(*this);

    //Filter bursts of callbacks. Happens a lot on Windows.
    timer.start();

    //This lambda will be called on a worker thread from filewatch
    fileWatcher = new filewatch::FileWatch<std::string>(initializationParameters.appResourcesFolder,
            [this, &initializationParameters](const std::string &path, const filewatch::Event change_type) {
                float timeSinceLast = timer.getElapsedSeconds();
                if (timeSinceLast > TIME_TO_DEBOUNCE_CALLBACK_S && change_type == filewatch::Event::modified) {
                    timer.start();
                    handleResourceChange(initializationParameters, path);
                }
            });
}

ResourceFilesWatcher::~ResourceFilesWatcher() {
    delete fileWatcher;
}

//Runs on worker thread from filewatch
void ResourceFilesWatcher::handleResourceChange(const InitializationParameters &initializationParameters,
                                                const std::string &path) {
    LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL, "ResourceFilesWatcher",
                                  "Resource file change detected: " + path);

    ReloadType type = ReloadType::UNKNOWN;

    if (path.find(initializationParameters.resourcesFile) != std::string::npos)
        type = ReloadType::RESOURCES_FILE;
    else if (path.find(initializationParameters.shadersFolder) != std::string::npos)
        type = ReloadType::SHADER;
    else if (path.find(initializationParameters.imagesFolder) != std::string::npos)
        type = ReloadType::IMAGE;
    else if (path.find(initializationParameters.materialsFolder) != std::string::npos)
        type = ReloadType::MATERIAL;
    else if (path.find(initializationParameters.meshesFolder) != std::string::npos)
        type = ReloadType::MESH;

    if (type != ReloadType::UNKNOWN) {
        ReloadData data;
        data.timeLeftToExecute = TIME_TO_WAIT_FOR_RELOAD_S;
        data.dir = Utils::getDirNameFromPath(path, Utils::getPathSeparator());
        data.file = Utils::getFileNameFromPath(path, Utils::getPathSeparator());

        std::lock_guard<std::mutex> lock(mutexMap);
        pendingReloads[type] = data;
    }
}

//Runs on Rendering thread
void ResourceFilesWatcher::onFrameEnd(float timePassed) {
    std::lock_guard<std::mutex> lock(mutexMap);

    for (auto it = pendingReloads.begin(); it != pendingReloads.end(); ) {
        auto &pair = *it;
        pair.second.timeLeftToExecute -= timePassed;

        if(pair.second.timeLeftToExecute <= 0.0f) {
            switch (pair.first) {
                case ReloadType::RESOURCES_FILE:
                    reloadResourcesFile(pair.second);
                    break;
                case ReloadType::IMAGE:
                    reloadImage(pair.second);
                    break;
                case ReloadType::SHADER:
                    reloadShader(pair.second);
                    break;
                case ReloadType::MATERIAL:
                    reloadMaterial(pair.second);
                    break;
                case ReloadType::MESH:
                    //TODO
                    break;
                default:
                    break;
            }

            it = pendingReloads.erase(it);
        }
        else {
            it++;
        }
    }
}


//On Windows sometimes we don't get a correct path for the changed file. On those cases we force a reloadAll() on the manager.
//It seems that the issue is related on how programs save the files to disk. Happens frequently saving files with IDEs but not with Notepad.
void ResourceFilesWatcher::reloadMaterial(const ReloadData &reloadData) const {
    MaterialManager &mm = Root::getInstance().getMaterialManager();
    std::string name = mm.getNameByFileName(reloadData.file);
    if (!name.empty())
        mm.reload(name);
    else
        mm.reloadAll();

    //Force rebinds of the GPU objects
    Root::getInstance().getRenderer().resetToBasicState();
}

void ResourceFilesWatcher::reloadImage(const ReloadData &reloadData) const {
    ImageManager &im = Root::getInstance().getImageManager();
    TextureManager &tm = Root::getInstance().getTextureManager();

    std::string name = im.getNameByFileName(reloadData.file);
    if (!name.empty()) {
        uint32 refCount = im.getReferenceCount(name);
        if (refCount > 0) {
            //If the image only has 1 reference we don't need to reload it. The dependent textures reload will do just that.
            //This avoids a double reload()
            if (refCount > 1)
                im.reload(name);

            std::vector<std::string> dependentTextures = tm.getTexturesWithImageName(name);
            for (auto &texName : dependentTextures)
                tm.reload(texName);
        }
    }
    else {
        tm.reloadAll();
    }

    //Force rebinds of the GPU objects
    Root::getInstance().getRenderer().resetToBasicState();
}

void ResourceFilesWatcher::reloadShader(const ReloadData &reloadData) const {
    GpuProgramStageManager &psm = Root::getInstance().getGpuProgramStageManager();
    GpuProgramManager &pm = Root::getInstance().getGpuProgramManager();

    std::string name = psm.getNameByFileName(reloadData.file);
    if (!name.empty()) {
        uint32 refCount = psm.getReferenceCount(name);
        if (refCount > 0) {
            //If the stage only has 1 reference we don't need to reload it. The dependent programs reload will do just that.
            //This avoids a double reload()
            if (refCount > 1)
                psm.reload(name);

            std::vector<std::string> dependentPrograms = pm.getProgramsWithStageName(name);
            for (auto &programName : dependentPrograms)
                pm.reload(programName);
        }
    }
    else {
        pm.reloadAll();
    }

    //Force rebinds of the GPU objects
    Root::getInstance().getRenderer().resetToBasicState();
}

void ResourceFilesWatcher::reloadResourcesFile(const ReloadData &reloadData) const {
    Root::getInstance().reloadResourcesFile();
}