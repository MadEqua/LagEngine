#pragma once

#include "ResourceManager.h"

namespace Lag
{
	enum GpuProgramStageType;
	class GpuProgramStage;

	enum GpuProgramUniformSize;
	enum GpuProgramUniformType;
	enum GpuProgramUniformSemantic;
	
	class GpuProgramStageManager : public ResourceManager
	{
	public:
		explicit GpuProgramStageManager(const std::string &folder);

		virtual bool create(const std::string &name, const std::string &file, GpuProgramStageType type) = 0;

	protected:
		virtual void parseResourceDescription(const TiXmlElement &element) override;	
		static GpuProgramStageType parseStageTypeFromString(const std::string &type);

		void parseUniformDeclaration(GpuProgramStage &stage, const TiXmlElement &element);
		static GpuProgramUniformType parseUniformTypeFromString(const std::string &type);
		static GpuProgramUniformSize parseUniformSizeFromString(const std::string &size);
		static GpuProgramUniformSemantic parseUniformSemanticFromString(const std::string &semantic);
	};
}