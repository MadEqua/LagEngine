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

Entity::~Entity()
{
	for (SubEntity *se : subEntities)
		delete se;
}

void Entity::addToRenderQueue(RenderQueue &renderQueue)
{
	for (SubEntity *se : subEntities)
		se->addToRenderQueue(renderQueue);
}

void Entity::render(Renderer &renderer, uint32 passId)
{
	for (SubEntity *se : subEntities)
		se->render(renderer, passId);
}