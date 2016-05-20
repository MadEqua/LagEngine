#pragma once

#include "ResourceManager.h"

namespace Lag
{
	class MaterialFactory;
	
	class MaterialManager : public ResourceManager
	{
	public:
		MaterialManager(MaterialFactory *factory);
		virtual ~MaterialManager();

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;
	};
}