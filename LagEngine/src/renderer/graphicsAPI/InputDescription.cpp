#include "InputDescription.h"

using namespace Lag;

InputDescription::InputDescription(const VertexDescription &vertexDescription, 
	const VertexBuffer &vertexBuffer) :
	vertexDescription(vertexDescription),
	vertexBuffer(vertexBuffer)
{
}

InputDescription::~InputDescription()
{
}