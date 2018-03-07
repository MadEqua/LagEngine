#include "MeshManager.h"

#include "../renderer/Mesh.h"

using namespace Lag;

MeshManager::MeshManager(MeshBuilder *builder) :
	XmlResourceManager("MeshManager", builder)
{
}

void MeshManager::initialize()
{
	defaultObject = get("defaultMesh");
}

MeshBuilder::MeshBuilder(const TiXmlDocument &resourcesXml, const std::string &resourceFolderPath) :
	XmlResourceBuilder("mesh", resourcesXml, resourceFolderPath)
{
}

Mesh* MeshBuilder::parseAndCreate(const std::string &name, const TiXmlElement &element) const
{
	return new Mesh(resourceFolderPath + '/' + parseFileAttribute(element));
}