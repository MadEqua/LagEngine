#pragma once

#include "ResourceManager.h"

namespace Lag
{
	class MeshFactory;
	
	class MeshManager : public ResourceManager
	{
	public:
		MeshManager(MeshFactory *factory);
		~MeshManager();

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;
	};
}