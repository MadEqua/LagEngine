#include "Entity.h"

#include "../renderer/Renderer.h"
#include "SubEntity.h"
#include "../renderer/Mesh.h"

using namespace Lag;

Entity::Entity(uint32 name, Handle<Material> defaultMaterial, Handle<Mesh> mesh) :
	SceneObject(name),
	defaultMaterial(defaultMaterial),
	mesh(mesh),
	renderMode(LAG_RENDER_MODE_TRIANGLES)
{
	for (SubMesh *sm : mesh->getSubMeshes())
	{
		SubEntity *se = new SubEntity(*this, *defaultMaterial.get(), *sm);
		subEntities.push_back(se);
	}
}

Entity::~Entity()
{
	for (SubEntity *se : subEntities)
		delete se;
}

void Entity::addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget)
{
	for (SubEntity *se : subEntities)
		se->addToRenderQueue(renderQueue, viewport, renderTarget);
}

void Entity::render(Renderer &renderer, RenderOperation &renderOperation)
{
	for (SubEntity *se : subEntities)
		se->render(renderer, renderOperation);
}