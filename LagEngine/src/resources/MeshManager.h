#pragma once

#include "ResourceManager.h"

namespace Lag
{
	class MeshManager : public ResourceManager
	{
	public:
		MeshManager();
		~MeshManager();

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;
	};
}