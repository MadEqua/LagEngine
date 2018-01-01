#pragma once

#include "../../renderer/graphicsAPI/GpuProgram.h"
#include "GL4Includes.h"

namespace Lag
{
	class GL4GpuProgram : public GpuProgram
	{
	public:
		GL4GpuProgram(const std::string &name, const std::vector<std::string> &names);
		GL4GpuProgram(const std::string &name, const std::vector<GpuProgramStage*> &stages);
		virtual ~GL4GpuProgram() override;

		virtual GpuProgramUniform* createUniform(const GpuProgramUniformDescription &description) const override;

		inline GLuint getHandle() const { return handle; }

	protected:
		virtual bool link() override;

		bool checkLinking() const;

		GLuint handle;
	};
}