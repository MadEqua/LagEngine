#include "InputDescriptionManager.h"

#include "../VertexDescription.h"
#include "../../io/log/LogManager.h"

using namespace Lag;

InputDescriptionMapKey::InputDescriptionMapKey(const VertexDescription &vd, const GpuBuffer *vb) :
	vertexDescription(vd), vertexBuffer(vb)
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


InputDescriptionManager::InputDescriptionManager() :
	Manager("InputDescriptionManager")
{
}

InputDescription* InputDescriptionManager::createInputDescription(const VertexDescription &vertexDescription, const GpuBuffer &vertexBuffer)
{
	InputDescriptionMapKey key(vertexDescription, &vertexBuffer);
	InputDescription *id = get(key);
	if (id == nullptr)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_DEBUG, LAG_LOG_VERBOSITY_NORMAL,
			"InputDescriptionManager", "Creating InputDescription.");

		id = internalCreateInputDescription(vertexDescription, vertexBuffer);
	}
	else
	{		
		LogManager::getInstance().log(LAG_LOG_TYPE_DEBUG, LAG_LOG_VERBOSITY_NORMAL,
			"InputDescriptionManager", "Reutilizing InputDescription.");
	}

	if(add(key, id) && load(key))
		return id;
	else 
		return nullptr;
}