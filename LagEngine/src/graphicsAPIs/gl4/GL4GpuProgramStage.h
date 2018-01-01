#pragma once

#include "../../renderer/graphicsAPI/GpuProgramStage.h"
#include "GL4Includes.h"

namespace Lag
{
	class GL4GpuProgramStage : public GpuProgramStage
	{
	public:
		GL4GpuProgramStage(const std::string &path, GpuProgramStageType type);
		virtual ~GL4GpuProgramStage() override;

		virtual bool compile() override;

		inline GLuint getHandle() const { return handle; }

	private:
		GLuint handle;
		GLenum convertTypeToGL();

		bool checkCompilation() const;
	};
}