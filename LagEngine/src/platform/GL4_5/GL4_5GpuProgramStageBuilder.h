#pragma once

#include "GpuProgramStageManager.h"
#include "GL4_5GpuProgramStage.h"

namespace Lag {
    class GL4_5GpuProgramStageBuilder : public GpuProgramStageBuilder {
    public:
        explicit GL4_5GpuProgramStageBuilder(const XmlResourceBuilderData &xmlResourceData) :
                GpuProgramStageBuilder(xmlResourceData) {}

        GpuProgramStage *parseAndCreate(const std::string &path, const TiXmlElement &element) const override {
            GL4GpuProgramStage *stage = new GL4GpuProgramStage(path + '/' + parseFileAttribute(element),
                                                               parseTypeAttribute(element));
            parseUniforms(*stage, element);
            return stage;
        }
    };
}