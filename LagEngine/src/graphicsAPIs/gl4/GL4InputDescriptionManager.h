#pragma 

#include "../../renderer/graphicsAPI/InputDescriptionManager.h"

namespace Lag
{
	class GpuBuffer;
	
	class GL4InputDescriptionManager : public InputDescriptionManager
	{
	protected:
		virtual InputDescription* internalCreateInputDescription(const VertexDescription &vertexDescription, const GpuBuffer &vertexBuffer) override;
	};
}