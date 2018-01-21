#pragma once

#include "XmlResourceManager.h"
#include "../renderer/graphicsAPI/GpuProgramStage.h"

namespace Lag
{
	enum GpuProgramStageType;

	enum GpuProgramUniformSize;
	enum GpuProgramUniformType;
	enum GpuProgramUniformSemantic;

	class GpuProgramStageBuilder : public XmlResourceBuilder<GpuProgramStage>
	{
	public:
		GpuProgramStageBuilder(const TiXmlDocument &resourcesXml, const std::string &resourceFolderPath);

	protected:
		void parseUniforms(GpuProgramStage &stage, const TiXmlElement &element) const;

		static GpuProgramStageType parseTypeAttribute(const TiXmlElement &element);
		static GpuProgramUniformType parseUniformTypeFromString(const std::string &type);
		static GpuProgramUniformSize parseUniformSizeFromString(const std::string &size);
		static GpuProgramUniformSemantic parseUniformSemanticFromString(const std::string &semantic);
	};
	

	class GpuProgramStageManager : public XmlResourceManager<GpuProgramStage>
	{
	public:
		explicit GpuProgramStageManager(GpuProgramStageBuilder *builder);
	};
}