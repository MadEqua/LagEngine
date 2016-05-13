#include "Entity.h"

#include "../renderer/Material.h"
#include "../renderer/Mesh.h"

using namespace Lag;

Entity::Entity(Material &material, Mesh &mesh) :
	material(material),
	mesh(mesh)
{
}

Entity::~Entity()
{
}
