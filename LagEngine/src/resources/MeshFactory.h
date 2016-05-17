#pragma once

#include "IResourceFactory.h"

#include <string>

namespace Lag
{
	class MeshFactory : public IResourceFactory
	{
	public:
		MeshFactory(const std::string &file);
		~MeshFactory();

		virtual Resource* create() const override;
	
	private:
		std::string file;
	};
}