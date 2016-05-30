#pragma once

#include "SceneObject.h"
#include "../renderer/Color.h"

namespace Lag
{
	class GpuProgram;
	
	class Light : public SceneObject
	{
	public:
		Light(const Color &color);
		virtual ~Light();

		inline const Color& getColor() const { return color; }
	protected:
		Color color;
	};
}