#pragma once

#include <string>

#include "../../resources/Resource.h"

namespace Lag
{
	enum GpuProgramStageType;
	
	/*
	* Abstract class representing one of the possible types of shaders that forms a GpuProgram.
	* Can be compiled independently and used on various GpuPrograms.
	* Each GraphicsAPI implementation will have its own concrete version.
	*/
	class GpuProgramStage : public Resource
	{
	public:
		GpuProgramStage(const std::string &filePath, GpuProgramStageType type);
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