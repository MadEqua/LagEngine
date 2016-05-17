#include "Entity.h"

#include "../renderer/Material.h"
#include "../renderer/Mesh.h"
#include "../Root.h"

using namespace Lag;

Entity::Entity(Material &material, Mesh &mesh) :
	material(material),
	mesh(mesh)
{
}

/*Entity::Entity(const std::string &meshName, const std::string &materialName) :
	material(Root::getInstance().getMaterialManager().get(materialName)),
	mesh(Root::getInstance().getMeshManager().get(meshName))
{
}*/

Entity::~Entity()
{
}