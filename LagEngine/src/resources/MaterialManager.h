#pragma once

#include "ResourceManager.h"

namespace Lag
{
	class MaterialManager : public ResourceManager
	{
	public:
		explicit MaterialManager(const std::string &folder);

		bool create(const std::string &name, const std::string &file);

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;
	};
}