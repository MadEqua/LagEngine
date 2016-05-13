#pragma once

#include <string>

#include "../../resources/Resource.h"
#include "GpuProgramStageType.h"

namespace Lag
{
	/*
	* Abstract class representing one of the possible types of shaders that forms a GpuProgram.
	* Can be compiled independently and used on various GpuPrograms.
	* Each GraphicsAPI implementation will have its own concrete version.
	*/
	class GpuProgramStage : public Resource
	{
	public:
		GpuProgramStage(const std::string &path, GpuProgramStageType type);
		virtual ~GpuProgramStage();

		inline GpuProgramStageType getType() const { return type; }

		virtual bool compile() = 0;

		virtual bool load() override;
		virtual void unload() override;

	protected:
		GpuProgramStageType type;
		std::string code;
	};
}