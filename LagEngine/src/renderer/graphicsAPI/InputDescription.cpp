#include "InputDescription.h"

using namespace Lag;

InputDescription::InputDescription(const VertexDescription &vertexDescription, const Handle<GpuBuffer> vertexBuffer) :
	vertexDescription(vertexDescription),
	vertexBuffer(vertexBuffer)
{
}