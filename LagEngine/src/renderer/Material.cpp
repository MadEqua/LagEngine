#include "Material.h"

#include "tinyxml/tinyxml.h"

#include "Root.h"
#include "TextureManager.h"
#include "GpuProgramManager.h"
#include "Renderer.h"

using namespace Lag;

Material::Material(const std::string &filePath) :
        XmlResource(filePath) {
}

/*Material::Material(const std::vector<std::string> &shaderStageNames, const std::vector<std::string> &textureNames) :
        shaderStageNames(shaderStageNames),
        textureNames(textureNames) {
}*/

void Material::bind() const {
    Renderer &renderer = Root::getInstance().getRenderer();
    renderer.bindGpuProgram(*gpuProgram);
    uint8 i = 0;
    for (auto &tex : textures)
        renderer.bindTexture(*tex, i++);

    renderer.bindDepthSettings(depthSettings);
    renderer.bindBlendingSettings(blendingSettings);
}

const std::vector<Texture *> *Material::getTexturesBySemantic(TextureSemantic semantic) const {
    auto it = texturesBySemantic.find(semantic);
    if (it != texturesBySemantic.end())
        return &it->second;
    else
        return nullptr;
}

bool Material::loadImplementation() {
    if (!parse())
        return false;

    return initialize();
}

bool Material::initialize() {
    Root &root = Root::getInstance();

    GpuProgramManager &programManager = root.getGpuProgramManager();
    gpuProgram = programManager.get(shaderStageNames);
    if (!gpuProgram.isValid())
        return false;

    TextureManager &texMan = root.getTextureManager();
    auto &textureBuilder = dynamic_cast<TextureBuilder &>(texMan.getBuilder());
    textureBuilder.setBuildFromXml();

    for (std::string &name : textureNames) {
        Handle<Texture> tex = texMan.get(name);
        if (tex.isValid()) {
            textures.push_back(tex);
            texturesBySemantic[tex->getTextureData().semantic].push_back(tex.get());
        }
        else
            return false;
    }

    return true;
}

bool Material::parse() {
    if (path.empty()) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "Material",
                                      "Trying to load Material with empty path.");
        return false;
    }

    TiXmlDocument doc(path);
    if (!doc.LoadFile()) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "Material",
                                      "Material file: " + path + " does not exist or is malformed.");
        return false;
    }

    const TiXmlElement *materialElement = doc.FirstChildElement();
    if (!materialElement) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "Material",
                                      "Material file: " + path + " does not contain <material> element.");
        return false;
    }

    //TODO: check for techniques and passes, not shaders
    for (const TiXmlElement *child = materialElement->FirstChildElement();
         child != nullptr;
         child = child->NextSiblingElement()) {

        if (child->ValueStr() == SHADER_XML_TAG) {
            const char *name = child->Attribute(NAME_XML_ATTR);
            if (name)
                shaderStageNames.emplace_back(name);
            else
                LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "Material",
                                              "Material file: " + path + " has a <" + SHADER_XML_TAG + "> element with no name");
        }
        else if (child->ValueStr() == TEXTURE_XML_TAG) {
            const char *name = child->Attribute(NAME_XML_ATTR);
            if (name)
                textureNames.emplace_back(name);
            else
                LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "Material",
                                              "Material file: " + path + " has a <" + TEXTURE_XML_TAG +"> element with no name");
        }
        else if (child->ValueStr() == BLENDING_XML_TAG) {
            const char *enabled = child->Attribute("enabled");
            if(enabled) {
                if(Utils::parseBool(enabled)) {
                    if (parseBlendingSettings(child)) {
                        blendingSettings.enableBlending = true;
                    }
                    else
                        return false;
                }
            }
            else {
                LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "Material",
                                              "Material file: " + path + " has a <" + BLENDING_XML_TAG + "> element with no child with no \"enabled\" attribute.");
                return false;
            }
        }
        else if (child->ValueStr() == DEPTH_XML_TAG) {
            const char *testEnabled = child->Attribute("testEnabled");
            const char *writingEnabled = child->Attribute("writingEnabled");
            const char *clamping = child->Attribute("clamping");
            const char *function = child->Attribute("function");
            if(testEnabled) {
                depthSettings.enableDepthTest = Utils::parseBool(testEnabled);

                if (depthSettings.enableDepthTest) {
                    if (writingEnabled && clamping && function) {
                        depthSettings.enableDepthWriting = Utils::parseBool(writingEnabled);
                        depthSettings.enableDepthClamping = Utils::parseBool(clamping);
                        depthSettings.comparisionFunction = parseComparisionFunction(function);
                    }
                    else {
                        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "Material",
                                                      "Material file: " + path + " has a <" + DEPTH_XML_TAG + "> element with \"testEnabled=true\"  but there are missing attributes.");
                    }
                }
            }
            else {
                LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "Material",
                                              "Material file: " + path + " has a <" + DEPTH_XML_TAG + "> element with missing \"testEnabled\" attribute.");
                return false;
            }
        }
    }
    return true;
}

bool Material::parseBlendingSettings(const TiXmlElement *blendingNode) {
    bool hasRgbSettings = false, hasAlphaSettings = false;
    for (const TiXmlElement *child = blendingNode->FirstChildElement();
         child != nullptr;
         child = child->NextSiblingElement()) {

        if(child->ValueStr() == "setting") {
            const char *type = child->Attribute("type");
            const char *sourceFunction = child->Attribute("sourceFunction");
            const char *destinationFunction = child->Attribute("destinationFunction");
            const char *equation = child->Attribute("equation");

            if(!type || !sourceFunction || !destinationFunction || !equation) {
                LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "Material",
                                              "Material file: " + path + " has a <setting> element with missing attributes.");
                return false;
            }

            bool isRgb = type == std::string("RGB");
            bool isRgba = type == std::string("RGBA");
            bool isAlpha = type == std::string("Alpha");

            if(isRgb || isRgba) {
                if(hasRgbSettings) {
                    LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "Material",
                                                  "Material file: " + path + R"( has multiple <setting> elements of type "RGB". Only using the first one.)");
                    continue;
                }
                hasRgbSettings = true;
                blendingSettings.settingRGB.sourceBlendingFunction = parseBlendingFunction(sourceFunction);
                blendingSettings.settingRGB.destinationBlendingFunction = parseBlendingFunction(destinationFunction);
                blendingSettings.settingRGB.blendingEquation = parseBlendingEquation(equation);
            }

            if(isAlpha || isRgba) {
                if(hasAlphaSettings) {
                    LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "Material",
                                                  "Material file: " + path + R"( has multiple <setting> elements of type "Alpha". Only using the first one.)");
                    continue;
                }
                hasAlphaSettings = true;
                blendingSettings.settingAlpha.sourceBlendingFunction = parseBlendingFunction(sourceFunction);
                blendingSettings.settingAlpha.destinationBlendingFunction = parseBlendingFunction(destinationFunction);
                blendingSettings.settingAlpha.blendingEquation = parseBlendingEquation(equation);
            }
        }
        else if(child->ValueStr() == "constantColor") {
            const char *r = child->Attribute("R");
            const char *g = child->Attribute("G");
            const char *b = child->Attribute("B");
            const char *a = child->Attribute("A");
            bool okColor = false;

            if(r && g && b) {
                okColor = true;
                blendingSettings.constantColor.setRGB(Utils::parseFloat(r),  Utils::parseFloat(g), Utils::parseFloat(b));
            }
            else {
                LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL, "Material",
                                              "Material file: " + path + " has a <constantColor> element that tries to define a RGB but is missing components.");
            }

            if(a) {
                okColor = true;
                blendingSettings.constantColor.setAlpha(Utils::parseFloat(a));
            }

            if(!okColor) {
                LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "Material",
                                              "Material file: " + path + " has a <constantColor> element that does not have an RGB or an Alpha");
                return false;
            }
        }
    }

    if(hasRgbSettings && hasAlphaSettings) {
        return true;
    }
    else {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "Material",
                                      "Material file: " + path + " has a <" + BLENDING_XML_TAG + "> element that does not have <setting> to rgb and alpha.");
        return false;
    }
}

void Material::unloadImplementation() {
    shaderStageNames.clear();
    textureNames.clear();
    texturesBySemantic.clear();
    textures.clear();
}

BlendingFunction Material::parseBlendingFunction(const std::string &function) {
    if(function == "Zero") return BlendingFunction::ZERO;
    else if(function == "One") return BlendingFunction::ONE;
    else if(function == "SourceColor" || function == "SrcColor") return BlendingFunction::SOURCE_COLOR;
    else if(function == "OneMinusSourceColor" || function == "OneMinusSrcColor") return BlendingFunction::ONE_MINUS_SOURCE_COLOR;
    else if(function == "DestinationColor" || function == "DstColor") return BlendingFunction::DESTINATION_COLOR;
    else if(function == "OneMinusDestinationColor" || function == "OneMinusDstColor") return BlendingFunction::ONE_MINUS_DESTINATION_COLOR;
    else if(function == "SourceAlpha" || function == "SrcAlpha") return BlendingFunction::SOURCE_ALPHA;
    else if(function == "OneMinusSourceAlpha" || function == "OneMinusSrcAlpha") return BlendingFunction::ONE_MINUS_SOURCE_ALPHA;
    else if(function == "DestinationAlpha" || function == "DstAlpha") return BlendingFunction::DESTINATION_ALPHA;
    else if(function == "OneMinusDestinationAlpha" || function == "OneMinusDstAlpha") return BlendingFunction::ONE_MINUS_DESTINATION_ALPHA;
    else if(function == "ConstantColor") return BlendingFunction::CONSTANT_COLOR;
    else if(function == "OneMinusConstantColor") return BlendingFunction::ONE_MINUS_CONSTANT_COLOR;
    else if(function == "ConstantAlpha") return BlendingFunction::CONSTANT_ALPHA;
    else if(function == "OneMinusConstantAlpha") return BlendingFunction::ONE_MINUS_CONSTANT_ALPHA;
    else if(function == "AlphaSaturate") return BlendingFunction::ALPHA_SATURATE;
    else if(function == "Source1Color" || function == "Src1Color") return BlendingFunction::SOURCE_1_COLOR;
    else if(function == "OneMinusSource1Color" || function == "OneMinusSrc1Color") return BlendingFunction::ONE_MINUS_SOURCE_1_COLOR;
    else if(function == "Source1Alpha" || function == "Src1Alpha") return BlendingFunction::SOURCE_1_ALPHA;
    else if(function == "OneMinusSource1Alpha" || function == "OneMinusSrc1Alpha") return BlendingFunction::ONE_MINUS_SOURCE_1_ALPHA;
    else {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "Material", "Error parsing BlendingFunction: " + function);
        return BlendingFunction::ZERO;
    }

}

BlendingEquation Material::parseBlendingEquation(const std::string &equation) {
    if(equation == "Add") return BlendingEquation::ADD;
    else if(equation == "SourceMinusDestination" || equation == "SrcMinusDst") return BlendingEquation::SOURCE_MINUS_DESTINATION;
    else if(equation == "DestinationMinusSource" || equation == "DstMinusSrc") return BlendingEquation::DESTINATION_MINUS_SOURCE;
    else if(equation == "Min") return BlendingEquation::MIN;
    else if(equation == "Max") return BlendingEquation::MAX;
    else {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "Material", "Error parsing BlendingEquation: " + equation);
        return BlendingEquation::ADD;
    }
}

ComparisionFunction Material::parseComparisionFunction(const std::string &function) {
    if(function == "Never") return ComparisionFunction::NEVER;
    else if(function == "Always") return ComparisionFunction::ALWAYS;
    else if(function == "Less") return ComparisionFunction::LESS;
    else if(function == "LessOrEqual" || function == "LEqual") return ComparisionFunction::LESS_OR_EQUAL;
    else if(function == "Greater") return ComparisionFunction::GREATER;
    else if(function == "GreaterOrEqual" || function == "GEqual") return ComparisionFunction::GREATER_OR_EQUAL;
    else if(function == "Equal") return ComparisionFunction::EQUAL;
    else if(function == "NotEqual" || function == "NEqual") return ComparisionFunction::NOT_EQUAL;
    else {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL, "Material", "Error parsing ComparisionFunction: " + function);
        return ComparisionFunction::NEVER;
    }
}
