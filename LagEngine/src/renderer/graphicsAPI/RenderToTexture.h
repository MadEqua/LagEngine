#pragma once

#include "../RenderTarget.h"
#include "../../core/Handle.h"

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
		RenderToTexture(uint32 width, uint32 height, RenderPhase renderPhase = LAG_RENDER_PHASE_COLOR);

		virtual void attachColorTexture(const ImageData &imageData, const TextureData &textureData, uint8 layer = 0, uint8 attachment = 0, uint8 mipmapLevel = 0) = 0;
		virtual void attachDepthStencilTexture(const ImageData &imageData, const TextureData &textureData, uint8 layer = 0, uint8 mipmapLevel = 0) = 0;
		virtual void attachDepthTexture(const ImageData &imageData, const TextureData &textureData, uint8 layer = 0, uint8 mipmapLevel = 0) = 0;
		virtual void attachStencilTexture(const ImageData &imageData, const TextureData &textureData, uint8 layer = 0, uint8 mipmapLevel = 0) = 0;

		virtual bool checkCompleteness() const = 0;

	protected:
		std::unordered_map<uint8, std::unordered_map<uint8, Handle<Texture>>> colorTextures;
		std::unordered_map<uint8, Handle<Texture>> depthTextures;
		std::unordered_map<uint8, Handle<Texture>> stencilTextures;
		std::unordered_map<uint8, Handle<Texture>> depthStencilTextures;

		Handle<Texture> createAndLoadTexture(const std::string &name, const ImageData &imageData, const TextureData &textureData);
	};
}
