#include "MeshManager.h"

#include "../renderer/Mesh.h"

using namespace Lag;

MeshManager::MeshManager(MeshBuilder *builder) :
	XmlResourceManager("MeshManager", builder)
{
}

void MeshManager::initializeFallbackObject()
{
	fallbackObject = get("defaultMesh");
}

MeshBuilder::MeshBuilder(const XmlResourceBuilderData &xmlResourceData) :
	XmlResourceBuilder(xmlResourceData)
{
}

Mesh* MeshBuilder::parseAndCreate(const std::string &path, const TiXmlElement &element) const
{
	return new Mesh(path + '/' + parseFileAttribute(element));
}