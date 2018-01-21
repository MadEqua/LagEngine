#include "InputDescriptionManager.h"

#include "../VertexDescription.h"

using namespace Lag;

InputDescriptionMapKey::InputDescriptionMapKey(const VertexDescription &vertexDescription, const GpuBuffer *vertexBuffer) :
	vertexDescription(vertexDescription), vertexBuffer(vertexBuffer)
{
}

bool InputDescriptionMapKey::operator==(const InputDescriptionMapKey &other) const
{
	return vertexBuffer == other.vertexBuffer && vertexDescription == other.vertexDescription;
}

std::size_t std::hash<Lag::InputDescriptionMapKey>::operator()(const Lag::InputDescriptionMapKey &k) const
{
	return std::hash<const GpuBuffer*>()(k.vertexBuffer) ^ static_cast<std::size_t>(k.vertexDescription);
}


InputDescriptionManager::InputDescriptionManager(InputDescriptionBuilder *builder) :
	Manager("InputDescriptionManager", builder)
{
}

InputDescription* InputDescriptionManager::get(const VertexDescription &vertexDescription, const GpuBuffer *vertexBuffer)
{
	InputDescriptionBuilder &inputDescriptionBuilder = static_cast<InputDescriptionBuilder&>(*builder);
	inputDescriptionBuilder.vertexBuffer = const_cast<GpuBuffer*>(vertexBuffer);
	inputDescriptionBuilder.vertexDescription = vertexDescription;
	return Manager::get(getName(vertexDescription, vertexBuffer));
}

InputDescription* InputDescriptionManager::get(const VertexDescription &vertexDescription, const GpuBuffer *vertexBuffer, ManagedObject &parent)
{
	InputDescriptionBuilder &inputDescriptionBuilder = static_cast<InputDescriptionBuilder&>(*builder);
	inputDescriptionBuilder.vertexBuffer = const_cast<GpuBuffer*>(vertexBuffer);
	inputDescriptionBuilder.vertexDescription = vertexDescription;
	return Manager::get(getName(vertexDescription, vertexBuffer), parent);
}

InputDescriptionMapKey InputDescriptionManager::getName(const VertexDescription &vertexDescription, const GpuBuffer *vertexBuffer)
{
	return InputDescriptionMapKey(vertexDescription, vertexBuffer);
}