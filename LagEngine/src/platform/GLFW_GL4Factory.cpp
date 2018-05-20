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

#include "../renderer/RenderTargetManager.h"
#include "../resources/TextureManager.h"
#include "../renderer/graphicsAPI/GpuProgramManager.h"
#include "../renderer/graphicsAPI/GpuBufferManager.h"
#include "../renderer/graphicsAPI/InputDescriptionManager.h"
#include "../resources/GpuProgramStageManager.h"

using namespace Lag;

IGraphicsAPI* GLFW_GL4Factory::getGraphicsAPI() const
{
	return new GL4GraphicsAPI();
}

InputManager* GLFW_GL4Factory::getInputManager(const RenderWindow &renderWindow) const
{
	return new GLFWInputManager(static_cast<const GLFWRenderWindow&>(renderWindow));
}

RenderTargetManager* GLFW_GL4Factory::getRenderTargetManager() const
{
	return new RenderTargetManager(new GLFWRenderTargetBuilder(), new GL4RenderTargetBuilder());
}

TextureManager* GLFW_GL4Factory::getTextureManager(const XmlResourceBuilderData &xmlResourceData) const
{
	return new TextureManager(new GL4TextureBuilder(xmlResourceData));
}

GpuBufferManager* GLFW_GL4Factory::getGpuBufferManager() const
{
	return new GpuBufferManager(new GL4GpuBufferBuilder());
}

GpuProgramManager* GLFW_GL4Factory::getGpuProgramManager() const
{
	return new GpuProgramManager(new GL4GpuProgramBuilder());
}

GpuProgramStageManager* GLFW_GL4Factory::getGpuProgramStageManager(const XmlResourceBuilderData &xmlResourceData) const
{
	return new GpuProgramStageManager(new GL4GpuProgramStageBuilder(xmlResourceData));
}

InputDescriptionManager* GLFW_GL4Factory::getInputDescriptionManager() const
{
	return new InputDescriptionManager(new GL4InputDescriptionBuilder());
}
