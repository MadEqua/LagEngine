#pragma once

#include "../../renderer/graphicsAPI/GpuProgramStage.h"
#include "GL4Includes.h"

namespace Lag
{
	class GL4GpuProgramStage : public GpuProgramStage
	{
	public:
		virtual bool compile() override;
		inline GLuint getHandle() const { return handle; }

	private:
		friend class GL4GpuProgramStageManager;
		GL4GpuProgramStage(const std::string &path, GpuProgramStageType type);

		GLuint handle;
		GLenum convertTypeToGL();

		virtual bool loadImplementation() override;
		virtual void unloadImplementation() override;

		bool checkCompilation() const;
	};
}