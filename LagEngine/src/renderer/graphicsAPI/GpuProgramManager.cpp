#include "GpuProgramManager.h"
#include "../../io/log/LogManager.h"

using namespace Lag;

GpuProgramManager::GpuProgramManager() :
	Manager("GpuProgramManager")
{
	LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
		"GpuProgramManager", "Initialized successfully.");
}

GpuProgramManager::~GpuProgramManager()
{
	LogManager::getInstance().log(LAG_LOG_OUT_FILE, LAG_LOG_VERBOSITY_NORMAL, LAG_LOG_TYPE_INFO,
		"GpuProgramManager", "Destroyed successfully.");
}