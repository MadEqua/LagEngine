#include "Material.h"

#include "../Root.h"

using namespace Lag;

Material::Material(const std::string &filePath) :
	Resource(filePath)
{
}

Material::~Material()
{
}

bool Material::load()
{
	Root &root = Root::getInstance();
	
	//todo: parse material file
	//create gpuprograms on its manager



	loaded = true;
	return true;
}

void Material::unload()
{

}