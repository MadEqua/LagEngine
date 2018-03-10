#pragma once

#include "XmlResourceManager.h"
#include "../renderer/Material.h"

namespace Lag
{
	class MaterialBuilder : public XmlResourceBuilder<Material>
	{
	public:
		MaterialBuilder(const TiXmlDocument &resourcesXml, const std::string &resourceFolderPath);

		virtual Material* parseAndCreate(const std::string &name, const TiXmlElement &element) const override;
	};
	

	class MaterialManager : public XmlResourceManager<Material>
	{
	public:
		explicit MaterialManager(MaterialBuilder *builder);
		virtual void initializeFallbackObject() override;
	};
}