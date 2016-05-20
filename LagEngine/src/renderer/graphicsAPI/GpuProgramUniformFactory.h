#pragma once

#include "../../core/IFactory.h"

namespace Lag
{
	class GpuProgramUniform;
	enum GpuProgramUniformType;
	enum GpuProgramUniformSemantic;
	enum GpuProgramUniformSize;
	
	/*
	* All GraphicsAPI implementations have to provide a factory to create concrete GpuProgramUniforms.
	* This makes possible an API agnostic GpuProgramStageUniforms class.
	*/
	class GpuProgramUniformFactory : public IFactory<GpuProgramUniform>
	{
	public:
		GpuProgramUniformFactory() {}
		virtual ~GpuProgramUniformFactory() {}

		virtual GpuProgramUniform* create() const = 0;

		GpuProgramUniformType type;
		GpuProgramUniformSemantic semantic;
		GpuProgramUniformSize size;
		void *dataLocation;
	};
}
