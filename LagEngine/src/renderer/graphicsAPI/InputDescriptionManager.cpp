#include "InputDescriptionManager.h"

#include "../VertexDescription.h"
#include "InputDescription.h"
#include "../VertexData.h"

#include "../../io/log/LogManager.h"

using namespace Lag;

InputDescriptionManager::InputDescriptionManager()
{
	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		"InputDescriptionManager", "Initialized successfully.");
}

InputDescriptionManager::~InputDescriptionManager()
{
	for (auto &pair : inputDescriptions)
		delete pair.second;

	for (auto vd : vertexDescriptions)
		delete vd;

	LogManager::getInstance().log(LAG_LOG_TYPE_INFO, LAG_LOG_VERBOSITY_NORMAL,
		"InputDescriptionManager", "Destroyed successfully.");
}

InputDescription* InputDescriptionManager::getInputDescription(const VertexDescription &vertexDescription,
	const GpuBuffer &vertexBuffer)
{
	MapKey key(&vertexDescription, &vertexBuffer);	
	auto it = inputDescriptions.find(key);
	if (it != inputDescriptions.end())
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_DEBUG, LAG_LOG_VERBOSITY_NORMAL,
			"InputDescriptionManager", "Reutilizing InputDescription.");
		return it->second;
	}
	else
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_DEBUG, LAG_LOG_VERBOSITY_NORMAL,
			"InputDescriptionManager", "Creating InputDescription.");
		
		return createInputDescription(vertexDescription, vertexBuffer);
	}
}

VertexDescription& InputDescriptionManager::createVertexDescription()
{
	VertexDescription *vd = new VertexDescription();
	vertexDescriptions.push_back(vd);
	return *vd;
}



InputDescriptionManager::MapKey::MapKey(const VertexDescription *vd, const GpuBuffer *vb) :
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
	return std::hash<const GpuBuffer*>()(k.vertexBuffer) ^
		static_cast<std::size_t>(*k.vertexDescription);
}