#pragma once

#include "XmlResourceManager.h"
#include "../renderer/Mesh.h"
#include <string>

namespace Lag
{
	class MeshBuilder : public XmlResourceBuilder<Mesh>
	{
	public:
		MeshBuilder(const XmlResourceBuilderData &xmlResourceData);

		virtual Mesh* parseAndCreate(const std::string &path, const TiXmlElement &element) const override;
	};

	
	class MeshManager : public XmlResourceManager<Mesh>
	{
	public:
		explicit MeshManager(MeshBuilder *builder);
		virtual void initializeFallbackObject() override;
	};
}