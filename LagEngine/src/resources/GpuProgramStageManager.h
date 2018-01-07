#pragma once

#include "XmlResourceManager.h"
#include "../renderer/graphicsAPI/GpuProgramStage.h"

namespace Lag
{
	enum GpuProgramStageType;
	class GpuProgramStage;

	enum GpuProgramUniformSize;
	enum GpuProgramUniformType;
	enum GpuProgramUniformSemantic;
	
	class GpuProgramStageManager : public XmlResourceManager<GpuProgramStage>
	{
	public:
		explicit GpuProgramStageManager(const std::string &folder);
		bool create(const std::string &name, const std::string &file, GpuProgramStageType type);

	protected:
		virtual GpuProgramStage* internalCreate(const std::string &file, GpuProgramStageType type) = 0;

		virtual void parseResourceDescription(const TiXmlElement &element) override;	
		static GpuProgramStageType parseStageTypeFromString(const std::string &type);

		void parseUniformDeclaration(GpuProgramStage &stage, const TiXmlElement &element);
		static GpuProgramUniformType parseUniformTypeFromString(const std::string &type);
		static GpuProgramUniformSize parseUniformSizeFromString(const std::string &size);
		static GpuProgramUniformSemantic parseUniformSemanticFromString(const std::string &semantic);
	};
}