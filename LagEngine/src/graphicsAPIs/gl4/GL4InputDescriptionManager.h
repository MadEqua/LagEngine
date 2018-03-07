#pragma 

#include "../../renderer/graphicsAPI/InputDescriptionManager.h"
#include "GL4InputDescription.h"

namespace Lag
{
	class GpuBuffer;

	class GL4InputDescriptionBuilder : public InputDescriptionBuilder
	{
	public:
		virtual GL4InputDescription* build(const InputDescriptionMapKey &name) const override
		{
			return new GL4InputDescription(vertexDescription, vertexBuffer);
		}
	};
	

	class GL4InputDescriptionManager : public InputDescriptionManager
	{
	public:
		GL4InputDescriptionManager() :
			InputDescriptionManager(new GL4InputDescriptionBuilder()) {}
	};
}