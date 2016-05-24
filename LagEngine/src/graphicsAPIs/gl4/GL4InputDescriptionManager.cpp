#include "GL4InputDescriptionManager.h"
#include "GL4InputDescription.h"

using namespace Lag;

GL4InputDescriptionManager::GL4InputDescriptionManager()
{
}

GL4InputDescriptionManager::~GL4InputDescriptionManager()
{
}

InputDescription* GL4InputDescriptionManager::createInputDescription(const VertexDescription &vertexDescription, 
	const VertexBuffer &vertexBuffer)
{
	return new GL4InputDescription(vertexDescription, vertexBuffer);
}