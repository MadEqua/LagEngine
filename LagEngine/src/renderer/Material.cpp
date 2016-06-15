#include "Material.h"

#include "../Root.h"
#include "../resources/TextureManager.h"
#include "../renderer/graphicsAPI/Texture.h"
#include "../renderer/graphicsAPI/GpuProgramManager.h"
#include "../renderer/graphicsAPI/GpuProgram.h"
#include "../io/log/LogManager.h"
#include "../io/tinyxml/tinyxml.h"
#include "../Root.h"
#include "Renderer.h"

using namespace Lag;

Material::Material(const std::string &filePath) :
	Resource(filePath), renderer(Root::getInstance().getRenderer())
{
}

Material::Material(const std::vector<std::string> shaderStageNames,
	const std::vector<std::string> texureNames) :
	renderer(Root::getInstance().getRenderer()),
	shaderStageNames(shaderStageNames),
	textureNames(texureNames)

{
	initialize();
}

Material::~Material()
{
}

void Material::bind() const
{
	renderer.bindGpuProgram(*gpuProgram);
	uint32 i = 0;
	for (auto tex : textures)
		renderer.bindTexture(*tex, i++);
}

const std::vector<Texture*>* Material::getTexturesBySemantic(TextureSemantic semantic) const
{ 
	auto it = texturesBySemantic.find(semantic);
	if (it != texturesBySemantic.end())
		return &it->second;
	else
		return nullptr;
}

bool Material::loadImplementation()
{
	if (!parse())
		return false;

	return initialize();
}

bool Material::initialize()
{
	Root &root = Root::getInstance();

	std::string gpuProgramName;
	GpuProgram::generateName(shaderStageNames, gpuProgramName);

	GpuProgramManager &manager = root.getGpuProgramManager();

	if (!manager.contains(gpuProgramName))
	{
		if (!manager.create(gpuProgramName, shaderStageNames))
			return false;
	}
	gpuProgram = static_cast<GpuProgram*>(manager.get(gpuProgramName));

	TextureManager &texMan = Root::getInstance().getTextureManager();
	for (std::string name : textureNames)
	{
		Texture *tex = static_cast<Texture*>(texMan.get(name));
		if (tex != nullptr)
		{
			textures.push_back(tex);
			texturesBySemantic[tex->getTextureData().semantic].push_back(tex);
		}
	}

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

		if (child->ValueStr() == "texture")
		{
			const char* name = child->Attribute("name");
			if (name)
				textureNames.push_back(name);
		}
	}
	return true;
}

void Material::unloadImplementation()
{
}
