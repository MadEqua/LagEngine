#pragma once

#include "IResourceFactory.h"
#include "../renderer/Material.h"

#include <string>

namespace Lag
{
	class MaterialFactory : public IResourceFactory
	{
	public:
		MaterialFactory() {}
		virtual ~MaterialFactory() {}

		virtual Material* create() const override { return new Material(file); }

		std::string file;
	};
}