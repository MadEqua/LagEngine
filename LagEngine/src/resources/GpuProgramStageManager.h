#pragma once

#include "Types.h"
#include "XmlResourceManager.h"
#include "GpuProgramStage.h"

namespace Lag {
    enum class GpuProgramStageType : uint8;
    enum class GpuProgramUniformSize : uint8;
    enum class GpuProgramUniformType : uint8;
    enum class GpuProgramUniformSemantic : uint8;

    class GpuProgramStageBuilder : public XmlResourceBuilder<GpuProgramStage> {
    public:
        explicit GpuProgramStageBuilder(const XmlResourceBuilderData &xmlResourceData);

    protected:
        void parseUniforms(GpuProgramStage &stage, const TiXmlElement &element) const;

        static GpuProgramStageType parseTypeAttribute(const TiXmlElement &element);
        static GpuProgramUniformType parseUniformTypeFromString(const std::string &type);
        static GpuProgramUniformSize parseUniformSizeFromString(const std::string &size);
        static GpuProgramUniformSemantic parseUniformSemanticFromString(const std::string &semantic);
    };


    class GpuProgramStageManager : public XmlResourceManager<GpuProgramStage> {
    public:
        explicit GpuProgramStageManager(GpuProgramStageBuilder *builder);
    };
}