#include "MaterialFactory.h"
#include "../renderer/Material.h"

using namespace Lag;

MaterialFactory::MaterialFactory()
{
}

MaterialFactory::~MaterialFactory()
{
}

void MaterialFactory::setFilePath(const std::string &filePath) 
{
	this->filePath = filePath;
}

Resource* MaterialFactory::create() const
{
	Material *mat = new Material(filePath);
	return mat;
}