#pragma once

#include <string>

class TiXmlDocument;

namespace Lag
{
	class IGraphicsAPI;
	class InputManager;
	class RenderTargetManager;
	class TextureManager;
	class GpuBufferManager;
	class GpuProgramManager;
	class InputDescriptionManager;
	class GpuProgramStageManager;
	class RenderWindow;


	/*
	* Factory to create all the platform specific Managers and classes. 
	* By platform we mean the Windowing system and the Graphics API.
	*/
	class IPlatformFactory
	{
	public:
		virtual ~IPlatformFactory() = default;

		virtual IGraphicsAPI* getGraphicsAPI() const = 0;
		virtual InputManager* getInputManager(const RenderWindow &renderWindow) const = 0;
		virtual RenderTargetManager* getRenderTargetManager() const = 0;
		virtual TextureManager* getTextureManager(const TiXmlDocument &resourcesXml) const = 0;
		virtual GpuBufferManager* getGpuBufferManager() const = 0;
		virtual GpuProgramManager* getGpuProgramManager() const = 0;
		virtual GpuProgramStageManager* getGpuProgramStageManager(const TiXmlDocument &resourcesXml, const std::string &resourceFolderPath) const = 0;
		virtual InputDescriptionManager* getInputDescriptionManager() const = 0;
	};
}