#pragma once

#include "ResourceManager.h"

namespace Lag
{
	class MeshManager : public ResourceManager
	{
	public:
		MeshManager();
		~MeshManager();

		void create(const std::string &name, const std::string &file);

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;
	};
}