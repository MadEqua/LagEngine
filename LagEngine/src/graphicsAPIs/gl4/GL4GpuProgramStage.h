#pragma once

#include "../../renderer/graphicsAPI/GpuProgramStage.h"
#include "GL4Includes.h"

namespace Lag
{
	class GL4GpuProgramStage : public GpuProgramStage
	{
	public:
		GL4GpuProgramStage(const std::string &path, GpuProgramStageType type);

		virtual bool compile() override;

		inline GLuint getHandle() const { return handle; }

	private:
		GLuint handle;
		GLenum convertTypeToGL();

		virtual bool loadImplementation() override;
		virtual void unloadImplementation() override;

		bool checkCompilation() const;
	};
}