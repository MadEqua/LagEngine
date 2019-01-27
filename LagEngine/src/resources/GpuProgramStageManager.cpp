#include "GpuProgramStageManager.h"

using namespace Lag;

GpuProgramStageManager::GpuProgramStageManager(GpuProgramStageBuilder *builder) :
        XmlResourceManager("GpuProgramStageManager", builder) {
}


GpuProgramStageBuilder::GpuProgramStageBuilder(const XmlResourceBuilderData &xmlResourceData) :
        XmlResourceBuilder(xmlResourceData) {
}

void GpuProgramStageBuilder::parseUniforms(GpuProgramStage &stage, const TiXmlElement &element) const {
    for (const TiXmlElement *child = element.FirstChildElement();
         child != nullptr;
         child = child->NextSiblingElement()) {

        if (child->ValueStr() == "uniform") {
            const char *name = child->Attribute(NAME_XML_ATTR);
            const char *semanticString = child->Attribute("semantic");

            if (!name || !semanticString) {
                LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "GpuProgramStageBuilder",
                                              "An <uniform> element on a shader declaration does not contain all required attributes: <name> and <semantic>.");
                return;
            }

            GpuProgramUniformSize size;
            GpuProgramUniformType type;

            GpuProgramUniformSemantic semantic = parseUniformSemanticFromString(semanticString);

            switch (semantic) {
                case GpuProgramUniformSemantic::MODEL_MATRIX:
                case GpuProgramUniformSemantic::MODELVIEW_MATRIX:
                case GpuProgramUniformSemantic::MVP_MATRIX:
                case GpuProgramUniformSemantic::VIEW_MATRIX:
                case GpuProgramUniformSemantic::VIEWPROJECTION_MATRIX:
                case GpuProgramUniformSemantic::PROJECTION_MATRIX:
                    size = GpuProgramUniformSize::SIZE_4;
                    type = GpuProgramUniformType::MATRIX;
                    break;

                case GpuProgramUniformSemantic::POINT_LIGHT_COUNT:
                case GpuProgramUniformSemantic::DIR_LIGHT_COUNT:
                case GpuProgramUniformSemantic::TEXTURE_CUSTOM:
                    size = GpuProgramUniformSize::SIZE_1;
                    type = GpuProgramUniformType::UINT32;
                    break;

                case GpuProgramUniformSemantic::TEXTURE_DIFFUSE:
                case GpuProgramUniformSemantic::TEXTURE_NORMAL:
                    size = GpuProgramUniformSize::SIZE_1;
                    type = GpuProgramUniformType::INT32;
                    break;

                case GpuProgramUniformSemantic::POINT_LIGHT_POSITIONS:
                case GpuProgramUniformSemantic::DIR_LIGHT_DIRECTIONS:
                case GpuProgramUniformSemantic::POINT_LIGHT_COLORS: //TODO: colors don't need to be floats
                case GpuProgramUniformSemantic::DIR_LIGHT_COLORS:
                case GpuProgramUniformSemantic::POINT_LIGHT_ATTENUATIONS:
                    size = GpuProgramUniformSize::SIZE_3;
                    type = GpuProgramUniformType::FLOAT;
                    break;


                case GpuProgramUniformSemantic::NORMAL_WORLD_MATRIX:
                case GpuProgramUniformSemantic::NORMAL_VIEW_MATRIX:
                    size = GpuProgramUniformSize::SIZE_3;
                    type = GpuProgramUniformType::MATRIX;
                    break;

                case GpuProgramUniformSemantic::TIMER:
                    size = GpuProgramUniformSize::SIZE_1;
                    type = GpuProgramUniformType::FLOAT;
                    break;

                case GpuProgramUniformSemantic::VIEWPORT_SIZE:
                    size = GpuProgramUniformSize::SIZE_2;
                    type = GpuProgramUniformType::UINT32;
                    break;

                case GpuProgramUniformSemantic::CUSTOM: {
                    const char *typeString = child->Attribute("type");
                    const char *sizeString = child->Attribute("size");

                    if (!typeString || !sizeString) {
                        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "GpuProgramStageBuilder",
                                                      "An <uniform> declared as custom does not have all required attributes declared (type and size).");
                        return;
                    }

                    type = parseUniformTypeFromString(typeString);
                    size = parseUniformSizeFromString(sizeString);
                }
                break;

                default:
                    continue;
            }
            stage.addUniformDescription(name, semantic, size, type);
        }
    }
}

GpuProgramUniformType GpuProgramStageBuilder::parseUniformTypeFromString(const std::string &type) {
    if (type == "bool") return GpuProgramUniformType::BOOL;
    else if (type == "float") return GpuProgramUniformType::FLOAT;
    else if (type == "int32" || type == "int") return GpuProgramUniformType::INT32;
    else if (type == "uint32" || type == "uint") return GpuProgramUniformType::UINT32;
    else if (type == "matrix") return GpuProgramUniformType::MATRIX;
    else return GpuProgramUniformType::UNKNOWN;
}

GpuProgramUniformSize GpuProgramStageBuilder::parseUniformSizeFromString(const std::string &size) {
    if (size == "1") return GpuProgramUniformSize::SIZE_1;
    else if (size == "2") return GpuProgramUniformSize::SIZE_2;
    else if (size == "3") return GpuProgramUniformSize::SIZE_3;
    else if (size == "4") return GpuProgramUniformSize::SIZE_4;
    else return GpuProgramUniformSize::SIZE_UNKNOWN;
}

GpuProgramUniformSemantic GpuProgramStageBuilder::parseUniformSemanticFromString(const std::string &semantic) {
    if (semantic == "ModelMatrix") return GpuProgramUniformSemantic::MODEL_MATRIX;
    else if (semantic == "ModelViewMatrix" || semantic ==  "MvMatrix") return GpuProgramUniformSemantic::MODELVIEW_MATRIX;
    else if (semantic == "MvpMatrix") return GpuProgramUniformSemantic::MVP_MATRIX;
    else if (semantic == "ViewMatrix") return GpuProgramUniformSemantic::VIEW_MATRIX;
    else if (semantic == "ViewProjectionMatrix" || semantic == "VpMatrix") return GpuProgramUniformSemantic::VIEWPROJECTION_MATRIX;
    else if (semantic == "ProjectionMatrix") return GpuProgramUniformSemantic::PROJECTION_MATRIX;
    else if (semantic == "NormalWorldMatrix" || semantic == "NormalMatrix") return GpuProgramUniformSemantic::NORMAL_WORLD_MATRIX;
    else if (semantic == "NormalViewMatrix") return GpuProgramUniformSemantic::NORMAL_VIEW_MATRIX;

    else if (semantic == "PointLightCount") return GpuProgramUniformSemantic::POINT_LIGHT_COUNT;
    else if (semantic == "PointLightPositions") return GpuProgramUniformSemantic::POINT_LIGHT_POSITIONS;
    else if (semantic == "PointLightColors") return GpuProgramUniformSemantic::POINT_LIGHT_COLORS;
    else if (semantic == "PointLightAttenuations") return GpuProgramUniformSemantic::POINT_LIGHT_ATTENUATIONS;

    else if (semantic == "DirectionalLightCount") return GpuProgramUniformSemantic::DIR_LIGHT_COUNT;
    else if (semantic == "DirectionalLightDirections") return GpuProgramUniformSemantic::DIR_LIGHT_DIRECTIONS;
    else if (semantic == "DirectionalLightColors") return GpuProgramUniformSemantic::DIR_LIGHT_COLORS;

    else if (semantic == "TextureDiffuse") return GpuProgramUniformSemantic::TEXTURE_DIFFUSE;
    else if (semantic == "TextureNormal") return GpuProgramUniformSemantic::TEXTURE_NORMAL;

    else if (semantic == "Timer") return GpuProgramUniformSemantic::TIMER;

    else if (semantic == "ViewportSize") return GpuProgramUniformSemantic::VIEWPORT_SIZE;

    else if (semantic == "Custom") return GpuProgramUniformSemantic::CUSTOM;
    else return GpuProgramUniformSemantic::CUSTOM;
}

GpuProgramStageType GpuProgramStageBuilder::parseTypeAttribute(const TiXmlElement &element) {
    const char *type = element.Attribute("type");

    if (!type) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "GpuProgramStageBuilder",
                                      "A <shader> element on the Resources file does not contain the required attribute: <type>.");
        return GpuProgramStageType::UNKNOWN;
    }

    std::string typeStr(type);
    if (typeStr == "Vertex") return GpuProgramStageType::VERTEX;
    else if (typeStr == "TesselationControl") return GpuProgramStageType::TESS_CONTROL;
    else if (typeStr == "TesselationEvaluation") return GpuProgramStageType::TESS_EVALUATION;
    else if (typeStr == "Geometry") return GpuProgramStageType::GEOMETRY;
    else if (typeStr == "Fragment") return GpuProgramStageType::FRAGMENT;
    else return GpuProgramStageType::UNKNOWN;
}