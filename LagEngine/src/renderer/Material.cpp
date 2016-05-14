#include "Material.h"

#include "../Root.h"
#include "../resources/GpuProgramManager.h"
#include "../resources/GpuProgramFactory.h"
#include "../io/log/LogManager.h"
#include "../io/tinyxml/tinyxml.h"

using namespace Lag;

Material::Material(const std::string &filePath) :
	Resource(filePath)
{
}

Material::~Material()
{
}

bool Material::load()
{
	Root &root = Root::getInstance();
	
	if (!parse())
		return false;

	//TODO: set a fixed order to avoid duplicates
	std::string combinedName;
	for (std::string &stageName : shaderStageNames)
		combinedName += stageName;

	GpuProgramFactory gpuProgramFactory(shaderStageNames, root.getInitializationParameters().graphicsApiType);
	root.getGpuProgramManager().create(combinedName, gpuProgramFactory);

	loaded = true;
	return true;
}

bool Material::parse()
{
	if (path.empty())
		return false;
	
	TiXmlDocument doc(path);
	if (!doc.LoadFile())
	{
		LogManager::getInstance().log(LogOutput::FILE, NORMAL, ERROR, "Material",
			"Material file: " + path + " does not exist or is malformed.");
		return false;
	}

	const TiXmlElement *materialElement = nullptr;
	for (const TiXmlElement* child = doc.FirstChildElement();
		child != 0;
		child = child->NextSiblingElement())
	{
		if (child->ValueStr() == "material")
		{
			materialElement = child;
			break;
		}
	}

	if (!materialElement)
	{
		LogManager::getInstance().log(LogOutput::FILE, NORMAL, ERROR, "Material",
			"Material file: " + path + " does not contain <material> element.");
		return false;
	}

	//TODO: check for techniques and passes, not shaders
	for (const TiXmlElement* child = materialElement->FirstChildElement();
		child != NULL;
		child = child->NextSiblingElement())
	{
		if (child->ValueStr() == "shader")
			shaderStageNames.push_back(child->GetText());
	}

	return true;
}

void Material::unload()
{

}