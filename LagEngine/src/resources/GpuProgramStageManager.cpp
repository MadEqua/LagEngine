#include "GpuProgramStageManager.h"

#include "../io/log/LogManager.h"
#include "../graphicsAPIs/gl4/GL4GpuProgramStage.h"

#include "../io/tinyxml/tinyxml.h"

using namespace Lag;

GpuProgramStageManager::GpuProgramStageManager(const GpuInterfaceType gpuInterfaceType) :
	gpuInterfaceType(gpuInterfaceType)
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
			LogManager::getInstance().log(FILE, NORMAL, ERROR, "GpuProgramStageManager",
				"A <shader> element on the Resources file does not contain all required elements: <name>, <file> and <type>");
			return;
		}

		LogManager::getInstance().log(FILE, NORMAL, INFO, "GpuProgramStageManager",
			"GpuProgramStage " + name + " has been declared from Resources file.");

		//TODO: is this OK here?
		if (gpuInterfaceType == OPENGL_4)
		{
			GpuProgramStageType realType;
			if (type == "vertex") realType = VERTEX;
			else if (type == "tesselationControl") realType = TESSELLATION_CONTROL;
			else if (type == "tesselationEvaluation") realType = TESSELLATION_EVALUATION;
			else if (type == "geometry") realType = GEOMETRY;
			else if (type == "fragment") realType = FRAGMENT;

			GL4GpuProgramStage *gps = new GL4GpuProgramStage(file, realType);
			gps->setPath(file);
			add(const_cast<const std::string&>(name), gps);
		}
	}
}