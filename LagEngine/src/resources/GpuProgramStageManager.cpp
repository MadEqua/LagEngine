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
	const char* semanticString = element.Attribute("semantic");

	if (!name || !semanticString)
	{
		LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_ERROR, "GpuProgramStageManager",
			"An <uniform> element on a shader declaration does not contain all required attributes: <name> and <semantic>.");
		return;
	}

	GpuProgramUniformSize size;
	GpuProgramUniformType type;
	GpuProgramUniformSemantic semantic = parseUniformSemanticFromString(semanticString);
	switch (semantic)
	{
	case LAG_GPU_PROG_UNI_SEM_MODEL_MATRIX:
	case LAG_GPU_PROG_UNI_SEM_MODELVIEW_MATRIX:
	case LAG_GPU_PROG_UNI_SEM_MVP_MATRIX:
		size = LAG_GPU_PROG_UNIFORM_SIZE_4;
		type = LAG_GPU_PROG_UNIFORM_TYPE_MATRIX;
		break;

	case LAG_GPU_PROG_UNI_SEM_NORMAL_MATRIX:
		size = LAG_GPU_PROG_UNIFORM_SIZE_3;
		type = LAG_GPU_PROG_UNIFORM_TYPE_MATRIX;
		break;

	case LAG_GPU_PROG_UNI_SEM_CUSTOM:
	{
		const char* typeString = element.Attribute("type");
		const char* sizeString = element.Attribute("size");

		if (!typeString || !sizeString)
		{
			LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_ERROR, "GpuProgramStageManager",
				"An <uniform> declared as custom does not have all required attributes declared (type and size).");
			return;
		}

		type = parseUniformTypeFromString(typeString);
		size = parseUniformSizeFromString(sizeString);
	}
		break;

	default:
		return;
	}

	stage.addUniformDescription(name, semantic, size, type);
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
	if (semantic == "ModelMatrix") return LAG_GPU_PROG_UNI_SEM_MODEL_MATRIX;
	else if (semantic == "ModelViewMatrix") return LAG_GPU_PROG_UNI_SEM_MODELVIEW_MATRIX;
	else if (semantic == "MvpMatrix") return LAG_GPU_PROG_UNI_SEM_MVP_MATRIX;
	else if (semantic == "NormalMatrix") return LAG_GPU_PROG_UNI_SEM_NORMAL_MATRIX;
	else if (semantic == "Custom") return LAG_GPU_PROG_UNI_SEM_CUSTOM;
	else return LAG_GPU_PROG_UNI_SEM_CUSTOM;
}

GpuProgramStageType GpuProgramStageManager::parseStageTypeFromString(const std::string &type)
{
	if (type == "Vertex") return LAG_GPU_PROG_STAGE_TYPE_VERTEX;
	else if (type == "TtesselationControl") return LAG_GPU_PROG_STAGE_TYPE_TESS_CONTROL;
	else if (type == "TesselationEvaluation") return LAG_GPU_PROG_STAGE_TYPE_TESS_EVALUATION;
	else if (type == "Geometry") return LAG_GPU_PROG_STAGE_TYPE_GEOMETRY;
	else if (type == "Fragment") return LAG_GPU_PROG_STAGE_TYPE_FRAGMENT;
	else return LAG_GPU_PROG_STAGE_TYPE_UNKNOWN;
}