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

		virtual bool initialize() override;
		virtual void destroy() override;

		virtual void attachColorTexture(const Texture &texture, uint8 attachment = 0, uint8 mipmapLevel = 0) override;
		virtual void attachDepthStencilTexture(const Texture &texture, uint8 mipmapLevel = 0) override;
		virtual void attachDepthTexture(const Texture &texture, uint8 mipmapLevel = 0) override;
		virtual void attachStencilTexture(const Texture &texture, uint8 mipmapLevel = 0) override;

	private:
		GLuint handle;

		bool checkFboCompleteness() const;
	};
}