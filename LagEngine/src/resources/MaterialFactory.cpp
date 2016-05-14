#include "MaterialFactory.h"
#include "../renderer/Material.h"

using namespace Lag;

MaterialFactory::MaterialFactory(const std::string &filePath) :
	filePath(filePath)
{
}

MaterialFactory::~MaterialFactory()
{
}

Resource* MaterialFactory::create() const
{
	Material *mat = new Material(filePath);
	return mat;
}