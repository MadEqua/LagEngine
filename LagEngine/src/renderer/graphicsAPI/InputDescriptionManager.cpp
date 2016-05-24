#include "InputDescriptionManager.h"

#include "../VertexDescription.h"
#include "InputDescription.h"
#include "../VertexData.h"

#include "../../io/log/LogManager.h"

using namespace Lag;

InputDescriptionManager::InputDescriptionManager()
{
	LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
		"InputDescriptionManager", "Initialized successfully.");
}

InputDescriptionManager::~InputDescriptionManager()
{
	for (auto &pair : inputDescriptions)
		delete pair.second;

	LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
		"InputDescriptionManager", "Destroyed successfully.");
}

InputDescription* InputDescriptionManager::getInputDescription(const VertexDescription &vertexDescription,
	const VertexBuffer &vertexBuffer)
{
	MapKey key(&vertexDescription, &vertexBuffer);	
	auto it = inputDescriptions.find(key);
	if (it != inputDescriptions.end())
	{
		LogManager::getInstance().log(LAG_LOG_OUT_CONSOLE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_DEBUG,
			"InputDescriptionManager", "Reutilizing InputDescription.");
		return it->second;
	}
	else
	{
		LogManager::getInstance().log(LAG_LOG_OUT_CONSOLE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_DEBUG,
			"InputDescriptionManager", "Creating InputDescription.");
		
		return createInputDescription(vertexDescription, vertexBuffer);
	}
}


InputDescriptionManager::MapKey::MapKey(const VertexDescription *vd, const VertexBuffer *vb) :
	vertexDescription(vd), vertexBuffer(vb)
{
}

InputDescriptionManager::MapKey::~MapKey()
{
}

bool InputDescriptionManager::MapKey::operator==(const MapKey &other) const
{
	return vertexBuffer == other.vertexBuffer &&
		*vertexDescription == *(other.vertexDescription);
}

std::size_t InputDescriptionManager::MapKey::MapKeyHasher::operator()(const MapKey& k) const
{
	return std::hash<const VertexBuffer*>()(k.vertexBuffer) ^ 
		static_cast<std::size_t>(*k.vertexDescription);
}