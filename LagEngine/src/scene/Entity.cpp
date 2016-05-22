#include "Entity.h"

#include "SubEntity.h"
#include "../renderer/Mesh.h"

using namespace Lag;

Entity::Entity(Material &defaultMaterial, Mesh &mesh) :
	defaultMaterial(defaultMaterial),
	mesh(mesh)
{
	for (SubMesh *sm : mesh.getSubMeshes())
	{
		SubEntity *se = new SubEntity(defaultMaterial, *sm);
		subEntities.push_back(se);
	}
}

/*Entity::Entity(const std::string &meshName, const std::string &materialName) :
	material(Root::getInstance().getMaterialManager().get(materialName)),
	mesh(Root::getInstance().getMeshManager().get(meshName))
{
}*/

Entity::~Entity()
{
	for (SubEntity *se : subEntities)
		delete se;
}