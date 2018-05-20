#pragma once

#include "IPlatformFactory.h"

namespace Lag
{
	class GLFW_GL4Factory : public IPlatformFactory
	{
	public:
		virtual IGraphicsAPI* getGraphicsAPI() const override;
		virtual InputManager* getInputManager(const RenderWindow &renderWindow) const override;
		
		virtual RenderTargetBuilder* getWindowRenderTargetBuilder() const override;
		virtual RenderTargetBuilder* getTextureRenderTargetBuilder() const override;
		virtual TextureBuilder* getTextureBuilder(const XmlResourceBuilderData &xmlResourceData) const override;
		virtual GpuBufferBuilder* getGpuBufferBuilder() const override;
		virtual GpuProgramBuilder* getGpuProgramBuilder() const override;
		virtual GpuProgramStageBuilder* getGpuProgramStageBuilder(const XmlResourceBuilderData &xmlResourceData) const override;
		virtual InputDescriptionBuilder* getInputDescriptionBuilder() const override;
	};
}