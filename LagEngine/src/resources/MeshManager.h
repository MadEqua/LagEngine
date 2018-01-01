#pragma once

#include "ResourceManager.h"
#include <string>

namespace Lag
{
	class MeshManager : public ResourceManager
	{
	public:
		explicit MeshManager(const std::string &folder);

		bool create(const std::string &name, const std::string &file);

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;
	};
}