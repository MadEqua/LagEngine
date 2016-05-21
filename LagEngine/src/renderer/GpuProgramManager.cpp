#include "GpuProgramManager.h"

using namespace Lag;

GpuProgramManager::GpuProgramManager(IFactory<GpuProgram> *programFactory,
	GpuProgramUniformFactory *uniformFactory) :
	Manager(programFactory),
	uniformFactory(uniformFactory)
{
}

GpuProgramManager::~GpuProgramManager()
{
	for (auto &pair : uniforms)
		for (auto *ptr : pair.second)
			delete ptr;

	delete uniformFactory;
}

const std::vector<GpuProgramUniform*>* GpuProgramManager::getUniformList(const std::string &name) const
{

}