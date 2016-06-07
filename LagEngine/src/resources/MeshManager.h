#pragma once

#include "ResourceManager.h"
#include <string>

namespace Lag
{
	class MeshManager : public ResourceManager
	{
	public:
		MeshManager(const std::string &folder);
		virtual ~MeshManager();

		bool create(const std::string &name, const std::string &file);

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;
	};
}