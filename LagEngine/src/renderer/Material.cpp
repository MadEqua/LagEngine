#include "Material.h"

#include "../Root.h"
#include "../renderer/graphicsAPI/GpuProgramManager.h"
#include "../renderer/graphicsAPI/GpuProgram.h"
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

bool Material::loadImplementation()
{
	Root &root = Root::getInstance();
	
	if (!parse())
		return false;

	std::string gpuProgramName;
	GpuProgram::generateName(shaderStageNames, gpuProgramName);

	GpuProgramManager &manager = root.getGpuProgramManager();
	if (!manager.create(gpuProgramName, shaderStageNames))
		return false;

	gpuProgram = static_cast<GpuProgram*>(manager.get(gpuProgramName));

	return true;
}

bool Material::parse()
{
	if (path.empty())
		return false;
	
	TiXmlDocument doc(path);
	if (!doc.LoadFile())
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "Material",
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
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "Material",
			"Material file: " + path + " does not contain <material> element.");
		return false;
	}

	//TODO: check for techniques and passes, not shaders
	for (const TiXmlElement* child = materialElement->FirstChildElement();
		child != 0;
		child = child->NextSiblingElement())
	{
		if (child->ValueStr() == "shader")
		{
			const char* name = child->Attribute("name");
			if (name)
				shaderStageNames.push_back(name);
		}
	}
	return true;
}

void Material::unloadImplementation()
{
}