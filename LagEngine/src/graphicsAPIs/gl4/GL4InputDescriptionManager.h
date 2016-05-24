#pragma 

#include "../../renderer/graphicsAPI/InputDescriptionManager.h"

namespace Lag
{
	class GpuBuffer;
	
	class GL4InputDescriptionManager : public InputDescriptionManager
	{
	public:
		GL4InputDescriptionManager();
		virtual ~GL4InputDescriptionManager();

	protected:
		virtual InputDescription* createInputDescription(const VertexDescription &vertexDescription, 
			const GpuBuffer &vertexBuffer) override;
	};
}