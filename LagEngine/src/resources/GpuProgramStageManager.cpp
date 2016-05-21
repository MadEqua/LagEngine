#include "GpuProgramStageManager.h"

#include "../io/log/LogManager.h"
#include "../renderer/graphicsAPI/GpuProgramStage.h"

#include "../io/tinyxml/tinyxml.h"

using namespace Lag;

GpuProgramStageManager::GpuProgramStageManager()
{
}

GpuProgramStageManager::~GpuProgramStageManager()
{
}

void GpuProgramStageManager::parseResourceDescription(const TiXmlElement &element)
{
	if (element.ValueStr() == "shader")
	{
		std::string name, file, type;
		
		for (const TiXmlElement* child = element.FirstChildElement();
			child != NULL;
			child = child->NextSiblingElement())
		{
			if (child->ValueStr() == "name")
				name = child->GetText();
			else if (child->ValueStr() == "file")
				file = child->GetText();
			else if (child->ValueStr() == "type")
				type = child->GetText();
		}

		if (name.empty() || file.empty() || type.empty())
		{
			LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_ERROR, "GpuProgramStageManager",
				"A <shader> element on the Resources file does not contain all required elements: <name>, <file> and <type>");
			return;
		}

		LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO, "GpuProgramStageManager",
			"GpuProgramStage " + name + " has been declared from Resources file.");

		create(name, file, parseTypeFromString(type));
	}
}

GpuProgramStageType GpuProgramStageManager::parseTypeFromString(const std::string &type)
{
	if (type == "vertex") return LAG_GPU_PROG_STAGE_TYPE_VERTEX;
	else if (type == "tesselationControl") return LAG_GPU_PROG_STAGE_TYPE_TESS_CONTROL;
	else if (type == "tesselationEvaluation") return LAG_GPU_PROG_STAGE_TYPE_TESS_EVALUATION;
	else if (type == "geometry") return LAG_GPU_PROG_STAGE_TYPE_GEOMETRY;
	else if (type == "fragment") return LAG_GPU_PROG_STAGE_TYPE_FRAGMENT;
	else return LAG_GPU_PROG_STAGE_TYPE_UNKNOWN;
}