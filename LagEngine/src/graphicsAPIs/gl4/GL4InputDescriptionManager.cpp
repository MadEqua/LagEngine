#include "GL4InputDescriptionManager.h"
#include "GL4InputDescription.h"

using namespace Lag;

InputDescription* GL4InputDescriptionManager::internalCreateInputDescription(const VertexDescription &vertexDescription,
	const GpuBuffer &vertexBuffer)
{
	return new GL4InputDescription(vertexDescription, vertexBuffer);
}