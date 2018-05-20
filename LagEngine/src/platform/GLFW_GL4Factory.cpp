#pragma once

#include "GLFW_GL4Factory.h"

#include "GL4/GL4GraphicsAPI.h"
#include "GL4/GL4RenderTargetBuilder.h"
#include "GL4/GL4TextureBuilder.h"
#include "GL4/GL4GpuBufferBuilder.h"
#include "GL4/GL4GpuProgramBuilder.h"
#include "GL4/GL4GpuProgramStageBuilder.h"
#include "GL4/GL4InputDescriptionBuilder.h"

#include "GLFW/GLFWInputManager.h"
#include "GLFW/GLFWRenderTargetBuilder.h"
#include "GLFW/GLFWRenderWindow.h"

#include "../resources/RenderTargetManager.h"
#include "../resources/TextureManager.h"
#include "../resources/GpuProgramManager.h"
#include "../resources/InputDescriptionManager.h"
#include "../resources/GpuProgramStageManager.h"
#include "../resources/GpuBufferManager.h"


using namespace Lag;

IGraphicsAPI* GLFW_GL4Factory::getGraphicsAPI() const
{
	return new GL4GraphicsAPI();
}

InputManager* GLFW_GL4Factory::getInputManager(const RenderWindow &renderWindow) const
{
	return new GLFWInputManager(static_cast<const GLFWRenderWindow&>(renderWindow));
}

RenderTargetBuilder* GLFW_GL4Factory::getWindowRenderTargetBuilder() const
{
	return new GLFWRenderTargetBuilder();
}

RenderTargetBuilder* GLFW_GL4Factory::getTextureRenderTargetBuilder() const
{
	return new GL4RenderTargetBuilder();
}

TextureBuilder* GLFW_GL4Factory::getTextureBuilder(const XmlResourceBuilderData &xmlResourceData) const
{
	return new GL4TextureBuilder(xmlResourceData);
}

GpuBufferBuilder* GLFW_GL4Factory::getGpuBufferBuilder() const
{
	return new GL4GpuBufferBuilder();
}

GpuProgramBuilder* GLFW_GL4Factory::getGpuProgramBuilder() const
{
	return new GL4GpuProgramBuilder();
}

GpuProgramStageBuilder* GLFW_GL4Factory::getGpuProgramStageBuilder(const XmlResourceBuilderData &xmlResourceData) const
{
	return new GL4GpuProgramStageBuilder(xmlResourceData);
}

InputDescriptionBuilder* GLFW_GL4Factory::getInputDescriptionBuilder() const
{
	return new GL4InputDescriptionBuilder();
}
