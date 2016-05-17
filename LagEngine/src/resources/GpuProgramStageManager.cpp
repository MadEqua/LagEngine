#include "GpuProgramStageManager.h"

#include "../io/log/LogManager.h"
#include "../renderer/GraphicsApiType.h"
#include "GpuProgramStageFactory.h"
#include "../Root.h"

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
			LogManager::getInstance().log(LogOutput::FILE, LogVerbosity::NORMAL, LogPriority::ERROR, "GpuProgramStageManager",
				"A <shader> element on the Resources file does not contain all required elements: <name>, <file> and <type>");
			return;
		}

		LogManager::getInstance().log(LogOutput::FILE, LogVerbosity::NORMAL, LogPriority::INFO, "GpuProgramStageManager",
			"GpuProgramStage " + name + " has been declared from Resources file.");

		GpuProgramStageFactory factory(file, type, 
			Root::getInstance().getInitializationParameters().graphicsApiType);
		create(name, factory);
	}
}