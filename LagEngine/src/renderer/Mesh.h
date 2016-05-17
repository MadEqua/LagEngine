#pragma once

#include "../resources/Resource.h"

namespace Lag
{
	class Mesh : public Resource
	{
	public:
		Mesh();
		~Mesh();

		virtual bool load() override;
		virtual void unload() override;
	};
}