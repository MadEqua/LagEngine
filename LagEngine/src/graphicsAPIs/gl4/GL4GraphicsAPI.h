#pragma once

#include "../../Renderer/graphicsApi/IGraphicsApi.h"

#include "GL4Includes.h"

namespace Lag
{
	class GL4GraphicsAPI : public IGraphicsAPI
	{
	public:
		GL4GraphicsAPI();
		~GL4GraphicsAPI();

		virtual RenderToTexture* createRenderToTexture(uint32 width, uint32 height) override;

		virtual void renderVertices(RenderMode mode, uint32 first, uint32 count) override;
		virtual void renderIndexed(RenderMode mode, uint32 first, IndexType indexType, uint32 count, uint32 baseVertex = 0) override;

		virtual void clearColorBuffer(const Color &color) override;
		virtual void clearDepthBuffer(float value) override;
		virtual void clearStencilBuffer(int32 value) override;
		virtual void clearDepthAndStencilBuffer(float depth, int32 stencil) override;

		virtual void bindVertexBuffer(const GpuBuffer &vertexBuffer) override;
		virtual void bindIndexBuffer(const GpuBuffer &indexBuffer) override;
		virtual void bindGpuProgram(const GpuProgram &gpuProgram) override;
		virtual void bindInputDescription(const InputDescription &inputDescription) override;
		virtual void bindRenderTarget(const RenderTarget &renderTarget) override;
		virtual void bindViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
		virtual void bindTexture(const Texture &texture, uint8 unit = 0) override;

		virtual void setDepthTestEnabled(bool enabled) override;
		virtual void setDepthWritingEnabled(bool enabled) override;

		/*virtual void setTexture(uint32 unit, Texture & texture) override;
		virtual void setTexture(uint32 unit, const std::string & textureName) override;
		virtual void setStencilCheckEnabled(bool enabled) override;*/

	private:
		static GLenum convertRenderModeToGLenum(RenderMode renderMode);
	};
}