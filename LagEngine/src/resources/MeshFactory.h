#pragma once

#include "IResourceFactory.h"
#include "../renderer/Mesh.h"

#include <string>

namespace Lag
{
	class MeshFactory : public IResourceFactory
	{
	public:
		MeshFactory(){}
		virtual ~MeshFactory(){}

		virtual Mesh* create() const override { return new Mesh(file); }

		std::string file;
	};
}