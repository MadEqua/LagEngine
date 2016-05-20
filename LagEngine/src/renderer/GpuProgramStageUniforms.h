#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "../core/Manager.h"
#include "../Types.h"

namespace Lag
{
	class GpuProgramUniform;
	class GpuProgramUniformFactory;
	enum GpuProgramUniformType;
	enum GpuProgramUniformSemantic;
	enum GpuProgramUniformSize;
	
	/*
	* Represents a full set of uniforms declared on a GpuProgramStage.
	* Manages a contiguous block of memory that stores (tightly) the values of all the added Uniforms.
	* Each Uniform will have a pointer to its space in that memory.
	*/
	class GpuProgramStageUniforms : Manager<GpuProgramUniform>
	{
	public:
		GpuProgramStageUniforms(GpuProgramUniformFactory *factory);
		virtual ~GpuProgramStageUniforms();

		virtual bool create(const std::string &name) override;

		void sendToGpu() const;

	private:
		std::vector<byte> dataPool;
	};
}