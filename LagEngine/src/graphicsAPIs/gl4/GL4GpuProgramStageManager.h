#pragma once

#include "../../resources/GpuProgramStageManager.h"
#include "GL4GpuProgramStage.h"

namespace Lag
{
	class GL4GpuProgramStageBuilder : public GpuProgramStageBuilder
	{
	public:
		GL4GpuProgramStageBuilder::GL4GpuProgramStageBuilder(const TiXmlDocument &resourcesXml, const std::string &resourceFolderPath) :
			GpuProgramStageBuilder(resourcesXml, resourceFolderPath) {}
		
		virtual GpuProgramStage* parseAndCreate(const std::string &name, const TiXmlElement &element) const override
		{	
			GL4GpuProgramStage* stage = new GL4GpuProgramStage(resourceFolderPath + '/' + parseFileAttribute(element), parseTypeAttribute(element));
			parseUniforms(*stage, element);
			return stage;
		}
	};
	

	class GL4GpuProgramStageManager : public GpuProgramStageManager
	{
	public:
		explicit GL4GpuProgramStageManager(GL4GpuProgramStageBuilder *builder) : 
			GpuProgramStageManager(builder) {}
	};
}