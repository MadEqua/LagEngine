#pragma once

#include "ResourceManager.h"

namespace Lag
{
	class MaterialManager : public ResourceManager
	{
	public:
		MaterialManager();
		virtual ~MaterialManager();

		bool create(const std::string &name, const std::string &file);

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;
	};
}