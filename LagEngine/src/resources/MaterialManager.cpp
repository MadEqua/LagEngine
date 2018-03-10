#include "MaterialManager.h"

#include "../renderer/Material.h"

using namespace Lag;

MaterialManager::MaterialManager(MaterialBuilder *builder) :
	XmlResourceManager("MaterialManager", builder)
{
}

void MaterialManager::initializeFallbackObject()
{
	fallbackObject = get("defaultMaterial");
}

MaterialBuilder::MaterialBuilder(const TiXmlDocument &resourcesXml, const std::string &resourceFolderPath) :
	XmlResourceBuilder("material", resourcesXml, resourceFolderPath)
{
}

Material* MaterialBuilder::parseAndCreate(const std::string &name, const TiXmlElement &element) const
{
	return new Material(resourceFolderPath + '/' + parseFileAttribute(element));
}