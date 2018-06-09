#include "GLFW_GL4_5Factory.h"

#include "GL4_5GraphicsAPI.h"
#include "GL4_5RenderTargetBuilder.h"
#include "GL4_5TextureBuilder.h"
#include "GL4_5GpuBufferBuilder.h"
#include "GL4_5GpuProgramBuilder.h"
#include "GL4_5GpuProgramStageBuilder.h"
#include "GL4_5InputDescriptionBuilder.h"

#include "GLFWInputManager.h"
#include "GLFWRenderTargetBuilder.h"


using namespace Lag;

IGraphicsAPI *GLFW_GL4_5Factory::getGraphicsAPI() const {
    return new GL4_5GraphicsAPI();
}

InputManager *GLFW_GL4_5Factory::getInputManager(const RenderWindow &renderWindow) const {
    return new GLFWInputManager(dynamic_cast<const GLFWRenderWindow &>(renderWindow));
}

RenderTargetBuilder *GLFW_GL4_5Factory::getWindowRenderTargetBuilder() const {
    return new GLFWRenderTargetBuilder();
}

RenderTargetBuilder *GLFW_GL4_5Factory::getTextureRenderTargetBuilder() const {
    return new GL4_5RenderTargetBuilder();
}

TextureBuilder *GLFW_GL4_5Factory::getTextureBuilder(const XmlResourceBuilderData &xmlResourceData) const {
    return new GL4_5TextureBuilder(xmlResourceData);
}

GpuBufferBuilder *GLFW_GL4_5Factory::getGpuBufferBuilder() const {
    return new GL4_5GpuBufferBuilder();
}

GpuProgramBuilder *GLFW_GL4_5Factory::getGpuProgramBuilder() const {
    return new GL4_5GpuProgramBuilder();
}

GpuProgramStageBuilder *GLFW_GL4_5Factory::getGpuProgramStageBuilder(const XmlResourceBuilderData &xmlResourceData) const {
    return new GL4_5GpuProgramStageBuilder(xmlResourceData);
}

InputDescriptionBuilder *GLFW_GL4_5Factory::getInputDescriptionBuilder() const {
    return new GL4_5InputDescriptionBuilder();
}
