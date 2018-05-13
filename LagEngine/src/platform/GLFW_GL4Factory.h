#pragma once

#include "IPlatformFactory.h"

namespace Lag
{
	class GLFW_GL4Factory : public IPlatformFactory
	{
	public:
		virtual IGraphicsAPI* getGraphicsAPI() const override;
		virtual InputManager* getInputManager(const RenderWindow &renderWindow) const override;
		virtual RenderTargetManager* getRenderTargetManager() const override;
		virtual TextureManager* getTextureManager(const TiXmlDocument &resourcesXml) const override;
		virtual GpuBufferManager* getGpuBufferManager() const override;
		virtual GpuProgramManager* getGpuProgramManager() const override;
		virtual GpuProgramStageManager* getGpuProgramStageManager(const TiXmlDocument &resourcesXml, const std::string &resourceFolderPath) const override;
		virtual InputDescriptionManager* getInputDescriptionManager() const override;
	};
}