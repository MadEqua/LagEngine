#pragma once

#include "../../renderer/graphicsAPI/InputDescription.h"

namespace Lag
{
	class GL4InputDescription : public InputDescription
	{
	public:
		GL4InputDescription(const VertexDescription &vertexDescription, const GpuBuffer &vertexBuffer) :
			InputDescription(vertexDescription, vertexBuffer) {}
		virtual ~GL4InputDescription() {}

		virtual void bind() override {};
	};
}