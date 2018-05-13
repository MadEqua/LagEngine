#include "ResourceFilesWatcher.h"

#include "../Root.h"
#include <filewatch/filewatch.h>

#include "../resources/MaterialManager.h"
#include "../resources/ImageManager.h"
#include "../resources/TextureManager.h"
#include "../resources/GpuProgramStageManager.h"
#include "../renderer/graphicsAPI/GpuProgramManager.h"
#include "../renderer/Renderer.h"

using namespace Lag;

ResourceFilesWatcher::ResourceFilesWatcher() :
	fileWatcher(nullptr),
	shouldReloadMaterials(false),
	shouldReloadTextures(false),
	shouldReloadShaders(false)
{
}

ResourceFilesWatcher::~ResourceFilesWatcher()
{
	if(fileWatcher != nullptr)
		delete fileWatcher;
}

void ResourceFilesWatcher::run(const InitializationParameters &initializationParameters)
{
#ifdef _DEBUG

	Root::getInstance().getRenderer().registerObserver(*this);

	timer.start();

	fileWatcher = new filewatch::FileWatch<std::string>(initializationParameters.resourcesFolder,
		[this, &initializationParameters](const std::string &path, const filewatch::Event change_type) {

		uint32 timeSinceLast = timer.getElapsedMilis();
		if (timeSinceLast > 500 && change_type == filewatch::Event::modified)
		{
			timer.start();
			handleResourceChange(initializationParameters, path);
		}
	});

#endif
}

void ResourceFilesWatcher::handleResourceChange(const InitializationParameters &initializationParameters, const std::string &path)
{
	std::cout << "File changed: " + path + "\n";

	//TODO: on Windows paths are not complete (neither reliable). This forces us to do a reloadAll() :(
	if (path.find(initializationParameters.resourcesFile) != std::string::npos)
	{

	}
	else if (path.find(initializationParameters.shadersFolder) != std::string::npos)
	{
		shouldReloadShaders = true;
	}
	else if (path.find(initializationParameters.imagesFolder) != std::string::npos)
	{
		shouldReloadTextures = true;
	}
	else if (path.find(initializationParameters.materialsFolder) != std::string::npos)
	{
		shouldReloadMaterials = true;
	}
	else if (path.find(initializationParameters.meshesFolder) != std::string::npos)
	{
	}
}

void  ResourceFilesWatcher::onFrameEnd(float timePassed)
{
	Root &root = Root::getInstance();
	
	if (shouldReloadMaterials)
	{
		root.getMaterialManager().reloadAll();
		shouldReloadMaterials = false;
	}
	else if (shouldReloadTextures)
	{
		root.getImageManager().reloadAll();
		root.getTextureManager().reloadAll();
		shouldReloadTextures = false;
	}
	else if (shouldReloadShaders)
	{
		root.getGpuProgramStageManager().reloadAll();
		root.getGpuProgramManager().reloadAll();
		shouldReloadShaders = false;
	}
}