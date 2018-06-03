#pragma once

#include "GpuProgramManager.h"
#include "GL4_5GpuProgram.h"

namespace Lag {
    class GL4_5GpuProgramBuilder : public GpuProgramBuilder {
    public:
        virtual GpuProgram *build(const std::string &name) const override {
            return new GL4GpuProgram(name, stagesNames);
        }
    };
}