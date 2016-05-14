#pragma once

#include "IResourceFactory.h"

#include <string>

namespace Lag
{
	class MaterialFactory : public IResourceFactory
	{
	public:
		MaterialFactory();
		~MaterialFactory();

		void setFilePath(const std::string &filePath);

		virtual Resource* create() const override;

	private:
		std::string filePath;
	};
}