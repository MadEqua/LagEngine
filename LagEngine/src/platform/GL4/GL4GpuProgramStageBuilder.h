#pragma once

#include "../../resources/GpuProgramStageManager.h"
#include "GL4GpuProgramStage.h"

namespace Lag
{
	class GL4GpuProgramStageBuilder : public GpuProgramStageBuilder
	{
	public:
		GL4GpuProgramStageBuilder::GL4GpuProgramStageBuilder(const XmlResourceBuilderData &xmlResourceData) :
			GpuProgramStageBuilder(xmlResourceData) {}
		
		virtual GpuProgramStage* parseAndCreate(const std::string &path, const TiXmlElement &element) const override
		{	
			GL4GpuProgramStage* stage = new GL4GpuProgramStage(path + '/' + parseFileAttribute(element), parseTypeAttribute(element));
			parseUniforms(*stage, element);
			return stage;
		}
	};
}