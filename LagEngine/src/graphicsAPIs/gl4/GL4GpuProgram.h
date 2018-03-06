#pragma once

#include "../../renderer/graphicsAPI/GpuProgram.h"
#include "GL4Includes.h"

namespace Lag
{
	class GL4GpuProgram : public GpuProgram
	{
	public:
		virtual GpuProgramUniform* createUniform(const GpuProgramUniformDescription &description) const override;
		inline GLuint getHandle() const { return handle; }

	protected:
		friend class GL4GpuProgramBuilder;
		GL4GpuProgram(const std::string &name, const std::vector<std::string> &names);

		virtual bool link() override;
		bool checkLinking() const;

		virtual bool loadImplementation() override;
		virtual void unloadImplementation() override;

		GLuint handle;
	};
}