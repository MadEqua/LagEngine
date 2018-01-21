#include "GpuProgramStageManager.h"

#include "../io/log/LogManager.h"
#include "../renderer/graphicsAPI/GpuProgramStage.h"

#include "../io/tinyxml/tinyxml.h"

using namespace Lag;

GpuProgramStageManager::GpuProgramStageManager(GpuProgramStageBuilder *builder) :
	XmlResourceManager("GpuProgramStageManager", builder)
{
}


GpuProgramStageBuilder::GpuProgramStageBuilder(const TiXmlDocument &resourcesXml, const std::string &resourceFolderPath) :
	XmlResourceBuilder("shader", resourcesXml, resourceFolderPath)
{
}

void GpuProgramStageBuilder::parseUniforms(GpuProgramStage &stage, const TiXmlElement &element) const
{
	for (const TiXmlElement* child = element.FirstChildElement();
		child != NULL;
		child = child->NextSiblingElement())
	{
		if (child->ValueStr() == "uniform")
		{
			const char* name = child->Attribute("name");
			const char* semanticString = child->Attribute("semantic");

			if (!name || !semanticString)
			{
				LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "GpuProgramStageBuilder",
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
			case LAG_GPU_PROG_UNI_SEM_VIEW_MATRIX:
			case LAG_GPU_PROG_UNI_SEM_VIEWPROJECTION_MATRIX:
			case LAG_GPU_PROG_UNI_SEM_PROJECTION_MATRIX:
				size = LAG_GPU_PROG_UNIFORM_SIZE_4;
				type = LAG_GPU_PROG_UNIFORM_TYPE_MATRIX;
				break;

			case LAG_GPU_PROG_UNI_SEM_POINT_LIGHT_COUNT:
			case LAG_GPU_PROG_UNI_SEM_DIR_LIGHT_COUNT:
			case LAG_GPU_PROG_UNI_SEM_TEXTURE_CUSTOM:
				size = LAG_GPU_PROG_UNIFORM_SIZE_1;
				type = LAG_GPU_PROG_UNIFORM_TYPE_UINT32;
				break;

			case LAG_GPU_PROG_UNI_SEM_TEXTURE_DIFFUSE:
			case LAG_GPU_PROG_UNI_SEM_TEXTURE_NORMAL:
				size = LAG_GPU_PROG_UNIFORM_SIZE_1;
				type = LAG_GPU_PROG_UNIFORM_TYPE_INT32;
				break;

			case LAG_GPU_PROG_UNI_SEM_POINT_LIGHT_POSITIONS:
			case LAG_GPU_PROG_UNI_SEM_DIR_LIGHT_DIRECTIONS:
			case LAG_GPU_PROG_UNI_SEM_POINT_LIGHT_COLORS: //TODO: colors need to be floats??
			case LAG_GPU_PROG_UNI_SEM_DIR_LIGHT_COLORS:
			case LAG_GPU_PROG_UNI_SEM_POINT_LIGHT_ATTENUATIONS:
				size = LAG_GPU_PROG_UNIFORM_SIZE_3;
				type = LAG_GPU_PROG_UNIFORM_TYPE_FLOAT;
				break;


			case LAG_GPU_PROG_UNI_SEM_NORMAL_WORLD_MATRIX:
			case LAG_GPU_PROG_UNI_SEM_NORMAL_VIEW_MATRIX:
				size = LAG_GPU_PROG_UNIFORM_SIZE_3;
				type = LAG_GPU_PROG_UNIFORM_TYPE_MATRIX;
				break;

			case LAG_GPU_PROG_UNI_SEM_CUSTOM:
			{
				const char* typeString = element.Attribute("type");
				const char* sizeString = element.Attribute("size");

				if (!typeString || !sizeString)
				{
					LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "GpuProgramStageBuilder",
						"An <uniform> declared as custom does not have all required attributes declared (type and size).");
					return;
				}

				type = parseUniformTypeFromString(typeString);
				size = parseUniformSizeFromString(sizeString);
			}
			break;

			default:
				break;
			}
			stage.addUniformDescription(name, semantic, size, type);
		}
	}
}

GpuProgramUniformType GpuProgramStageBuilder::parseUniformTypeFromString(const std::string &type)
{
	if (type == "bool") return LAG_GPU_PROG_UNIFORM_TYPE_BOOL;
	else if(type == "float") return LAG_GPU_PROG_UNIFORM_TYPE_FLOAT;
	else if (type == "int32") return LAG_GPU_PROG_UNIFORM_TYPE_INT32;
	else if (type == "matrix") return LAG_GPU_PROG_UNIFORM_TYPE_MATRIX;
	else return LAG_GPU_PROG_UNIFORM_TYPE_UNKNOWN;
}

GpuProgramUniformSize GpuProgramStageBuilder::parseUniformSizeFromString(const std::string &size)
{
	if (size == "1") return LAG_GPU_PROG_UNIFORM_SIZE_1;
	else if (size == "2") return LAG_GPU_PROG_UNIFORM_SIZE_2;
	else if (size == "3") return LAG_GPU_PROG_UNIFORM_SIZE_3;
	else if (size == "4") return LAG_GPU_PROG_UNIFORM_SIZE_4;
	else return LAG_GPU_PROG_UNIFORM_SIZE_UNKNOWN;
}

GpuProgramUniformSemantic GpuProgramStageBuilder::parseUniformSemanticFromString(const std::string &semantic)
{
	if (semantic == "ModelMatrix") return LAG_GPU_PROG_UNI_SEM_MODEL_MATRIX;
	else if (semantic == "ModelViewMatrix") return LAG_GPU_PROG_UNI_SEM_MODELVIEW_MATRIX;
	else if (semantic == "MvpMatrix") return LAG_GPU_PROG_UNI_SEM_MVP_MATRIX;
	else if (semantic == "ViewMatrix") return LAG_GPU_PROG_UNI_SEM_VIEW_MATRIX;
	else if (semantic == "ViewProjectionMatrix") return LAG_GPU_PROG_UNI_SEM_VIEWPROJECTION_MATRIX;
	else if (semantic == "ProjectionMatrix") return LAG_GPU_PROG_UNI_SEM_PROJECTION_MATRIX;
	else if (semantic == "NormalWorldMatrix") return LAG_GPU_PROG_UNI_SEM_NORMAL_WORLD_MATRIX;
	else if (semantic == "NormalViewMatrix") return LAG_GPU_PROG_UNI_SEM_NORMAL_VIEW_MATRIX;

	else if (semantic == "PointLightCount") return LAG_GPU_PROG_UNI_SEM_POINT_LIGHT_COUNT;
	else if (semantic == "PointLightPositions") return LAG_GPU_PROG_UNI_SEM_POINT_LIGHT_POSITIONS;
	else if (semantic == "PointLightColors") return LAG_GPU_PROG_UNI_SEM_POINT_LIGHT_COLORS;
	else if (semantic == "PointLightAttenuations") return LAG_GPU_PROG_UNI_SEM_POINT_LIGHT_ATTENUATIONS;

	else if (semantic == "DirectionalLightCount") return LAG_GPU_PROG_UNI_SEM_DIR_LIGHT_COUNT;
	else if (semantic == "DirectionalLightDirections") return LAG_GPU_PROG_UNI_SEM_DIR_LIGHT_DIRECTIONS;
	else if (semantic == "DirectionalLightColors") return LAG_GPU_PROG_UNI_SEM_DIR_LIGHT_COLORS;

	else if (semantic == "TextureDiffuse") return LAG_GPU_PROG_UNI_SEM_TEXTURE_DIFFUSE;
	else if (semantic == "TextureNormal") return LAG_GPU_PROG_UNI_SEM_TEXTURE_NORMAL;

	else if (semantic == "Custom") return LAG_GPU_PROG_UNI_SEM_CUSTOM;
	else return LAG_GPU_PROG_UNI_SEM_CUSTOM;
}

GpuProgramStageType GpuProgramStageBuilder::parseTypeAttribute(const TiXmlElement &element)
{
	const char* type = element.Attribute("type");

	if (!type)
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "GpuProgramStageBuilder",
			"A <shader> element on the Resources file does not contain the required attribute: <type>.");
		return LAG_GPU_PROG_STAGE_TYPE_UNKNOWN;
	}
	
	std::string typeStr(type);
	if (typeStr == "Vertex") return LAG_GPU_PROG_STAGE_TYPE_VERTEX;
	else if (typeStr == "TesselationControl") return LAG_GPU_PROG_STAGE_TYPE_TESS_CONTROL;
	else if (typeStr == "TesselationEvaluation") return LAG_GPU_PROG_STAGE_TYPE_TESS_EVALUATION;
	else if (typeStr == "Geometry") return LAG_GPU_PROG_STAGE_TYPE_GEOMETRY;
	else if (typeStr == "Fragment") return LAG_GPU_PROG_STAGE_TYPE_FRAGMENT;
	else return LAG_GPU_PROG_STAGE_TYPE_UNKNOWN;
}