#pragma 

#include "../../renderer/graphicsAPI/InputDescriptionManager.h"
#include "GL4InputDescription.h"

namespace Lag
{
	class GpuBuffer;
	
	class GL4InputDescriptionManager : public InputDescriptionManager
	{
	protected:
		virtual InputDescription* internalCreateInputDescription(const VertexDescription &vertexDescription, const GpuBuffer &vertexBuffer)
		{
			return new GL4InputDescription(vertexDescription, vertexBuffer);
		}
	};
}