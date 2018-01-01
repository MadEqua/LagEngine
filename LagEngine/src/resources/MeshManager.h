#pragma once

#include "XmlResourceManager.h"
#include "../renderer/Mesh.h"
#include <string>

namespace Lag
{
	class MeshManager : public XmlResourceManager<Mesh>
	{
	public:
		explicit MeshManager(const std::string &folder);

		bool create(const std::string &name, const std::string &file);

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;
	};
}