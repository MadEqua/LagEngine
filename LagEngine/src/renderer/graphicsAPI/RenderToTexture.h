#pragma once

#include "../RenderTarget.h"
#include <unordered_map>
#include <vector>

namespace Lag
{
	class Texture;
	struct ImageData;
	
	class RenderToTexture : public RenderTarget
	{
	public:
		RenderToTexture(uint32 width, uint32 height);
		virtual ~RenderToTexture();

		virtual void attachColorTexture(const Texture &texture, uint8 attachment = 0, uint8 mipmapLevel = 0) = 0;
		virtual void attachDepthStencilTexture(const Texture &texture, uint8 mipmapLevel = 0) = 0;
		virtual void attachDepthTexture(const Texture &texture, uint8 mipmapLevel = 0) = 0;
		virtual void attachStencilTexture(const Texture &texture, uint8 mipmapLevel = 0) = 0;

	protected:
		std::unordered_map<uint8, const Texture*> colorTextures;
		std::vector<const Texture*> depthTextures;
		std::vector<const Texture*> stencilTextures;
		std::vector<const Texture*> depthStencilTextures;
	};
}
