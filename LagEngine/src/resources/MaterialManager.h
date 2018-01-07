#pragma once

#include "XmlResourceManager.h"
#include "../renderer/Material.h"

namespace Lag
{
	class MaterialManager : public XmlResourceManager<Material>
	{
	public:
		explicit MaterialManager(const std::string &folder);
		bool create(const std::string &name, const std::string &file);

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;
	};
}