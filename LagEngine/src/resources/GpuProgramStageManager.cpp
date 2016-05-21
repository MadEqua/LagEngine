#include "GpuProgramStageManager.h"

#include "../io/log/LogManager.h"
#include "../renderer/graphicsAPI/GpuProgramStage.h"

#include "../io/tinyxml/tinyxml.h"

using namespace Lag;

GpuProgramStageManager::GpuProgramStageManager() :
	ResourceManager("GpuProgramStageManager")
{
}

GpuProgramStageManager::~GpuProgramStageManager()
{
}

void GpuProgramStageManager::parseResourceDescription(const TiXmlElement &element)
{
	if (element.ValueStr() == "shader")
	{
		const char* name = element.Attribute("name");
		const char* file = element.Attribute("file");
		const char* type = element.Attribute("type");
		
		if (!name || !file || !type)
		{
			LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_ERROR, "GpuProgramStageManager",
				"A <shader> element on the Resources file does not contain all required attributes: <name>, <file> and <type>.");
			return;
		}

		if (!create(name, file, parseStageTypeFromString(type)))
			return;

		GpuProgramStage *stage = static_cast<GpuProgramStage*>(get(name));

		for (const TiXmlElement* child = element.FirstChildElement();
			child != NULL;
			child = child->NextSiblingElement())
		{
			if (child->ValueStr() == "uniform")
				parseUniformDeclaration(*stage, *child);
		}
	}
}

void GpuProgramStageManager::parseUniformDeclaration(GpuProgramStage &stage, const TiXmlElement &element)
{
	const char* name = element.Attribute("name");
	const char* semantic = element.Attribute("semantic");

	if (!name || !semantic)
	{
		LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_ERROR, "GpuProgramStageManager",
			"A <uniform> element on a shader declaration does not contain all required attributes: <name> and <semantic>.");
		return;
	}

	const char* type = element.Attribute("type");
	const char* size = element.Attribute("size");

	stage.addUniformDescription(name, 
		parseUniformSemanticFromString(semantic),
		parseUniformSizeFromString(size != 0 ? size : ""),
		parseUniformTypeFromString(type != 0 ? type : ""));
}

GpuProgramUniformType GpuProgramStageManager::parseUniformTypeFromString(const std::string &type)
{
	if (type == "bool") return LAG_GPU_PROG_UNIFORM_TYPE_BOOL;
	else if(type == "float") return LAG_GPU_PROG_UNIFORM_TYPE_FLOAT;
	else if (type == "int32") return LAG_GPU_PROG_UNIFORM_TYPE_INT32;
	else if (type == "sampler") return LAG_GPU_PROG_UNIFORM_TYPE_SAMPLER;
	else if (type == "matrix") return LAG_GPU_PROG_UNIFORM_TYPE_MATRIX;
	else return LAG_GPU_PROG_UNIFORM_TYPE_UNKNOWN;
}

GpuProgramUniformSize GpuProgramStageManager::parseUniformSizeFromString(const std::string &size)
{
	if (size == "1") return LAG_GPU_PROG_UNIFORM_SIZE_1;
	else if (size == "2") return LAG_GPU_PROG_UNIFORM_SIZE_2;
	else if (size == "3") return LAG_GPU_PROG_UNIFORM_SIZE_3;
	else if (size == "4") return LAG_GPU_PROG_UNIFORM_SIZE_4;
	else return LAG_GPU_PROG_UNIFORM_SIZE_UNKNOWN;
}

GpuProgramUniformSemantic GpuProgramStageManager::parseUniformSemanticFromString(const std::string &semantic)
{
	if (semantic == "modelMatrix") return LAG_GPU_PROG_UNI_SEM_MODEL_MATRIX;
	else if (semantic == "modelViewMatrix") return LAG_GPU_PROG_UNI_SEM_MODELVIEW_MATRIX;
	else if (semantic == "mvpMatrix") return LAG_GPU_PROG_UNI_SEM_MVP_MATRIX;
	else if (semantic == "custom") return LAG_GPU_PROG_UNI_SEM_CUSTOM;
	else return LAG_GPU_PROG_UNI_SEM_CUSTOM;
}

GpuProgramStageType GpuProgramStageManager::parseStageTypeFromString(const std::string &type)
{
	if (type == "vertex") return LAG_GPU_PROG_STAGE_TYPE_VERTEX;
	else if (type == "tesselationControl") return LAG_GPU_PROG_STAGE_TYPE_TESS_CONTROL;
	else if (type == "tesselationEvaluation") return LAG_GPU_PROG_STAGE_TYPE_TESS_EVALUATION;
	else if (type == "geometry") return LAG_GPU_PROG_STAGE_TYPE_GEOMETRY;
	else if (type == "fragment") return LAG_GPU_PROG_STAGE_TYPE_FRAGMENT;
	else return LAG_GPU_PROG_STAGE_TYPE_UNKNOWN;
}