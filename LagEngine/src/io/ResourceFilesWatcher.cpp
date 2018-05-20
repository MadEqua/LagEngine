#include "ResourceFilesWatcher.h"

#include "../Root.h"
#include <filewatch/filewatch.h>

#include "../resources/MaterialManager.h"
#include "../resources/ImageManager.h"
#include "../resources/TextureManager.h"
#include "../resources/MeshManager.h"
#include "../resources/GpuProgramStageManager.h"
#include "../renderer/graphicsAPI/GpuProgramManager.h"
#include "../renderer/Renderer.h"

#include "../utils/Utils.h"

#include "log/LogManager.h"

using namespace Lag;

ResourceFilesWatcher::ResourceFilesWatcher(const InitializationParameters &initializationParameters) :
	fileWatcher(nullptr)
{
	Root::getInstance().getRenderer().registerObserver(*this);

	timer.start();

	fileWatcher = new filewatch::FileWatch<std::string>(initializationParameters.appResourcesFolder,
		[this, &initializationParameters](const std::string &path, const filewatch::Event change_type) {

		uint32 timeSinceLast = timer.getElapsedMilis();
		if (timeSinceLast > 100 && change_type == filewatch::Event::modified)
		{
			timer.start();
			handleResourceChange(initializationParameters, path);
		}
	});
}

ResourceFilesWatcher::~ResourceFilesWatcher()
{
	if(fileWatcher != nullptr)
		delete fileWatcher;
}

void ResourceFilesWatcher::handleResourceChange(const InitializationParameters &initializationParameters, const std::string &path)
{
	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_MINIMAL, "ResourceFilesWatcher",
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

	if(type != ReloadType::UNKNOWN)
	{
		ReloadData data;
		data.type = type;
		data.dir = Utils::getDirNameFromPath(path, Utils::getPathSeparator());
		data.file = Utils::getFileNameFromPath(path, Utils::getPathSeparator());

		std::lock_guard<std::mutex> lock(mutexSet);
		pendingReloads.insert(data);
	}
}

void ResourceFilesWatcher::onFrameEnd(float timePassed)
{
	Root &root = Root::getInstance();

	std::lock_guard<std::mutex> lock(mutexSet);

	for (auto &reloadData : pendingReloads)
	{
		switch (reloadData.type)
		{
		case ReloadType::RESOURCES_FILE:
			reloadResourcesFile(reloadData);
			break;
		case ReloadType::IMAGE:
			reloadImage(reloadData);
			break;
		case ReloadType::SHADER:
			reloadShader(reloadData);
			break;
		case ReloadType::MATERIAL:
			reloadMaterial(reloadData);
			break;
		case ReloadType::MESH:
			//TODO
			break;
		}
	}

	pendingReloads.clear();
}


//On Windows sometimes we don't get a correct path for the changed file. On those cases we force a reloadAll() on the manager.
//It seems that the issue is related on how programs save the files to disk. Happens frequently saving files with VS but not with Notepad.
void ResourceFilesWatcher::reloadMaterial(const ReloadData &reloadData) const
{
	MaterialManager &mm = Root::getInstance().getMaterialManager();
	std::string name = mm.getNameByFileName(reloadData.file);
	if (!name.empty())
		mm.reload(name);
	else
		mm.reloadAll();
}

void ResourceFilesWatcher::reloadImage(const ReloadData &reloadData) const
{
	ImageManager &im = Root::getInstance().getImageManager();
	TextureManager &tm = Root::getInstance().getTextureManager();

	std::string name = im.getNameByFileName(reloadData.file);
	if (!name.empty())
	{
		uint32 refCount = im.getReferenceCount(name);
		if (refCount > 0)
		{
			//If the image only has 1 reference we don't need to reload it. The dependent textures reload will do just that.
			//This avoids a double reload()
			if (refCount > 1)
				im.reload(name);

			std::vector<std::string> dependentTextures = tm.getTexturesWithImageName(name);
			for (auto &texName : dependentTextures)
				tm.reload(texName);
		}
	}
	else
	{
		tm.reloadAll();
	}
}

void ResourceFilesWatcher::reloadShader(const ReloadData &reloadData) const
{
	GpuProgramStageManager &psm = Root::getInstance().getGpuProgramStageManager();
	GpuProgramManager &pm = Root::getInstance().getGpuProgramManager();

	std::string name = psm.getNameByFileName(reloadData.file);
	if (!name.empty())
	{
		uint32 refCount = psm.getReferenceCount(name);
		if (refCount > 0)
		{
			//If the stage only has 1 reference we don't need to reload it. The dependent programs reload will do just that.
			//This avoids a double reload()
			if (refCount > 1)
				psm.reload(name);

			std::vector<std::string> dependentPrograms = pm.getProgramsWithStageName(name);
			for (auto &programName : dependentPrograms)
				pm.reload(programName);
		}
	}
	else
	{
		pm.reloadAll();
	}
}

void ResourceFilesWatcher::reloadResourcesFile(const ReloadData &reloadData) const
{
	Root::getInstance().reloadResourcesFile();
}