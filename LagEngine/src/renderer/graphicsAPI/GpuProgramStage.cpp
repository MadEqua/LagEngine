#include "GpuProgramStage.h"

#include "../../io/log/LogManager.h"
#include <fstream>

using namespace Lag;

GpuProgramStage::GpuProgramStage(const std::string &path, GpuProgramStageType type) :
	Resource(path),
	type(type)
{
}

GpuProgramStage::~GpuProgramStage()
{
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
		LogManager::getInstance().log(FILE, NORMAL, ERROR, "GpuProgramStage", "Cannot open code file: " + path);
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

	isLoaded = true;

	return true;
}