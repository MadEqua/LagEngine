#include "InputDescription.h"

using namespace Lag;

InputDescription::InputDescription(const VertexDescription &vertexDescription, const GpuBuffer &vertexBuffer) :
	vertexDescription(vertexDescription),
	vertexBuffer(vertexBuffer)
{
}