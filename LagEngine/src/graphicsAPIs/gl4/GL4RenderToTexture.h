#pragma once

#include "../../renderer/graphicsAPI/RenderToTexture.h"
#include "../../Types.h"
#include "GL4Includes.h"

namespace Lag
{
	class GL4RenderToTexture : public RenderToTexture
	{
	public:
		GL4RenderToTexture(uint32 width, uint32 height);
		virtual ~GL4RenderToTexture();

		virtual void attachColorTexture(const ImageData &imageData, const TextureData &textureData, uint8 layer = 0, uint8 attachment = 0, uint8 mipmapLevel = 0) override;
		virtual void attachDepthStencilTexture(const ImageData &imageData, const TextureData &textureData, uint8 layer = 0, uint8 mipmapLevel = 0) override;
		virtual void attachDepthTexture(const ImageData &imageData, const TextureData &textureData, uint8 layer = 0, uint8 mipmapLevel = 0) override;
		virtual void attachStencilTexture(const ImageData &imageData, const TextureData &textureData, uint8 layer = 0, uint8 mipmapLevel = 0) override;

		inline GLuint getHandle() const { return handle; }

	private:
		GLuint handle;

		std::string generateTextureName(const char* type, uint8 layer, uint8 attachment);

		virtual bool checkCompleteness() const override;
	};
}