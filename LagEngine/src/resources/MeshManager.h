#pragma once

#include "XmlResourceManager.h"
#include "../renderer/Mesh.h"
#include <string>

namespace Lag
{
	class MeshBuilder : public XmlResourceBuilder<Mesh>
	{
	public:
		MeshBuilder(const TiXmlDocument &resourcesXml, const std::string &resourceFolderPath);

		virtual Mesh* parseAndCreate(const std::string &name, const TiXmlElement &element) const override;
	};

	
	class MeshManager : public XmlResourceManager<Mesh>
	{
	public:
		explicit MeshManager(MeshBuilder *builder);
	};
}