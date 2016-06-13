#include "Entity.h"

#include "SubEntity.h"
#include "../renderer/Mesh.h"

using namespace Lag;

Entity::Entity(uint32 name, Material &defaultMaterial, Mesh &mesh) :
	SceneObject(name),
	defaultMaterial(defaultMaterial),
	mesh(mesh)
{
	for (SubMesh *sm : mesh.getSubMeshes())
	{
		SubEntity *se = new SubEntity(*this, defaultMaterial, *sm);
		subEntities.push_back(se);
	}
}

Entity::~Entity()
{
	for (SubEntity *se : subEntities)
		delete se;
}

void Entity::addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport)
{
	for (SubEntity *se : subEntities)
		se->addToRenderQueue(renderQueue, viewport);
}

void Entity::render(Renderer &renderer, RenderOperation &renderOperation)
{
	for (SubEntity *se : subEntities)
		se->render(renderer, renderOperation);
}