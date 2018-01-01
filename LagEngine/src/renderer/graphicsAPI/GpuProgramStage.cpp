#include "GpuProgramStage.h"

#include "../../io/log/LogManager.h"
#include <fstream>

using namespace Lag;

GpuProgramStage::GpuProgramStage(const std::string &filePath, GpuProgramStageType type) :
	XmlResource(filePath),
	type(type)
{
}

void GpuProgramStage::addUniformDescription(const std::string &name, GpuProgramUniformSemantic semantic,
	GpuProgramUniformSize size, GpuProgramUniformType type)
{
	GpuProgramUniformDescription desc;
	desc.name = name;
	desc.size = size;
	desc.type = type;
	desc.semantic = semantic;
	uniformDescriptions.push_back(desc);
}

bool GpuProgramStage::loadImplementation()
{
	std::ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"GpuProgramStage", "Cannot open code file: " + path);
		return false;
	}

	code.clear();
	std::string str;
	while (std::getline(file, str))
	{
		code += str + '\n';
	}
	
	file.close();

	if (!compile())
		return false;

	return true;
}

void GpuProgramStage::unloadImplementation()
{
}