#pragma once

#include "../RenderTarget.h"
#include <unordered_map>
#include <string>

namespace Lag
{
	class Texture;
	struct ImageData;
	struct TextureData;
	
	class RenderToTexture : public RenderTarget
	{
	public:
		RenderToTexture(uint32 width, uint32 height);

		virtual void attachColorTexture(const ImageData &imageData, const TextureData &textureData, uint8 layer = 0, uint8 attachment = 0, uint8 mipmapLevel = 0) = 0;
		virtual void attachDepthStencilTexture(const ImageData &imageData, const TextureData &textureData, uint8 layer = 0, uint8 mipmapLevel = 0) = 0;
		virtual void attachDepthTexture(const ImageData &imageData, const TextureData &textureData, uint8 layer = 0, uint8 mipmapLevel = 0) = 0;
		virtual void attachStencilTexture(const ImageData &imageData, const TextureData &textureData, uint8 layer = 0, uint8 mipmapLevel = 0) = 0;

		virtual bool initialize() override;
		virtual void destroy() override;

	protected:
		std::unordered_map<uint8, std::unordered_map<uint8, const Texture*>> colorTextures;
		std::unordered_map<uint8, const Texture*> depthTextures;
		std::unordered_map<uint8, const Texture*> stencilTextures;
		std::unordered_map<uint8, const Texture*> depthStencilTextures;

		const Texture* createAndLoadTexture(const std::string &name, const ImageData &imageData, const TextureData &textureData);

		virtual bool checkCompleteness() const = 0;
	};
}
