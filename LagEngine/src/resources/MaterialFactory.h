#pragma once

#include "IResourceFactory.h"

#include <string>

namespace Lag
{
	class MaterialFactory : public IResourceFactory
	{
	public:
		MaterialFactory(const std::string &filePath);
		~MaterialFactory();

		virtual Resource* create() const override;

	private:
		std::string filePath;
	};
}