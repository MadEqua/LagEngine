#pragma once

#include "XmlResourceManager.h"
#include "../renderer/Material.h"

namespace Lag
{
	class MaterialBuilder : public XmlResourceBuilder<Material>
	{
	public:
		MaterialBuilder(const XmlResourceBuilderData &xmlResourceData);

		virtual Material* parseAndCreate(const std::string &path, const TiXmlElement &element) const override;
	};
	

	class MaterialManager : public XmlResourceManager<Material>
	{
	public:
		explicit MaterialManager(MaterialBuilder *builder);
		virtual void initializeFallbackObject() override;
	};
}