#pragma once

#include "../../renderer/graphicsAPI/GpuProgram.h"
#include "GL4Includes.h"

#include <string>

namespace Lag
{
	class GL4GpuProgram : public GpuProgram
	{
	public:
		GL4GpuProgram(const std::string &name, const std::vector<std::string> &names);
		GL4GpuProgram(const std::string &name, const std::vector<GpuProgramStage*> &stages);
		virtual ~GL4GpuProgram();

		virtual GpuProgramUniform* createUniform(const GpuProgramUniformDescription &description, void* dataLocation) const override;

		virtual void bind() const override;

	protected:
		virtual bool link() override;

		bool checkLinking() const;

		GLuint handle;
	};
}