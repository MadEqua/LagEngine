#include "Material.h"

#include "../Root.h"
#include "../resources/TextureManager.h"
#include "../renderer/graphicsAPI/Texture.h"
#include "../renderer/graphicsAPI/GpuProgramManager.h"
#include "../resources/GpuProgramStageManager.h"
#include "../renderer/graphicsAPI/GpuProgramStage.h"
#include "../renderer/graphicsAPI/GpuProgram.h"
#include "../io/log/LogManager.h"
#include "tinyxml/tinyxml.h"
#include "../Root.h"
#include "Renderer.h"
#include "../Constants.h"

using namespace Lag;

Material::Material(const std::string &filePath) :
	XmlResource(filePath)
{
}

Material::Material(const std::vector<std::string> shaderStageNames, const std::vector<std::string> textureNames) :
	shaderStageNames(shaderStageNames),
	textureNames(textureNames)
{
}

void Material::bind() const
{
	Renderer &renderer = Root::getInstance().getRenderer();
	renderer.bindGpuProgram(*gpuProgram);
	uint32 i = 0;
	for (auto &tex : textures)
		renderer.bindTexture(*tex.get(), i++);
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

	GpuProgramManager &programManager = root.getGpuProgramManager();
	gpuProgram = programManager.get(shaderStageNames);
	if (!gpuProgram.isValid())
		return false;

	TextureManager &texMan = root.getTextureManager();
	TextureBuilder &textureBuilder = static_cast<TextureBuilder&>(texMan.getBuilder());
	textureBuilder.setBuildFromXml();

	for (std::string &name : textureNames)
	{
		Handle<Texture> tex = texMan.get(name);
		if (tex.isValid())
		{
			textures.push_back(tex);
			texturesBySemantic[tex->getTextureData().semantic].push_back(tex.get());
		}
		else
			return false;
	}

	return true;
}

bool Material::parse()
{
	if (path.empty())
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "Material",
			"Trying to load Material with empty path.");
		return false;
	}
	
	TiXmlDocument doc(path);
	if (!doc.LoadFile())
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "Material",
			"Material file: " + path + " does not exist or is malformed.");
		return false;
	}

	const TiXmlElement *materialElement = doc.FirstChildElement();
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
		if (child->ValueStr() == SHADER_XML_TAG)
		{
			const char* name = child->Attribute(NAME_XML_ATTR);
			if (name)
				shaderStageNames.push_back(name);
			else
				LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL, "Material",
					"Material file: " + path + " has a <shader> element with no name");
		}

		if (child->ValueStr() == TEXTURE_XML_TAG)
		{
			const char* name = child->Attribute(NAME_XML_ATTR);
			if (name)
				textureNames.push_back(name);
			else
				LogManager::getInstance().log(LAG_LOG_TYPE_WARNING, LAG_LOG_VERBOSITY_NORMAL, "Material",
					"Material file: " + path + " has a <texture> element with no name");
		}
	}
	return true;
}

void Material::unloadImplementation()
{
	shaderStageNames.clear();
	textureNames.clear();
	texturesBySemantic.clear();
	textures.clear();
}