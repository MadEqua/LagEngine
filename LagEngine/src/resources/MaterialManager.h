#pragma once

#include "ResourceManager.h"

namespace Lag
{
	class MaterialManager : public ResourceManager
	{
	public:
		MaterialManager();
		virtual ~MaterialManager();

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;
	};
}