#include "MeshFactory.h"

#include "../renderer/Mesh.h"

using namespace Lag;

MeshFactory::MeshFactory(const std::string &file) :
	file(file)
{
}


MeshFactory::~MeshFactory()
{
}


Resource* MeshFactory::create() const
{
	Mesh *m = new Mesh();
	return m;
}