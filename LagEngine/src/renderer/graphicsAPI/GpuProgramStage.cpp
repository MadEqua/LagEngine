#include "GpuProgramStage.h"

#include "../../io/log/LogManager.h"
#include <fstream>

using namespace Lag;

GpuProgramStage::GpuProgramStage(const std::string &filePath, GpuProgramStageType type) :
	Resource(filePath),
	type(type)
{
}

GpuProgramStage::~GpuProgramStage()
{
}

void GpuProgramStage::addUniformDescription(const std::string &name, GpuProgramUniformSize size, 
	GpuProgramUniformType type, GpuProgramUniformSemantic semantic)
{
	GpuProgramUniformDescription desc;
	desc.name = name;
	desc.size = size;
	desc.type = type;
	desc.semantic = semantic;
	uniformDescriptions.push_back(desc);
}

void GpuProgramStage::unload()
{
}

bool GpuProgramStage::load()
{
	std::ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_ERROR, 
			"GpuProgramStage", "Cannot open code file: " + path);
		return false;
	}

	code.clear();
	std::string str;
	while (std::getline(file, str))
	{
		code += str + '\n';
		file.close();
	}

	if (!compile())
		return false;

	loaded = true;

	return true;
}