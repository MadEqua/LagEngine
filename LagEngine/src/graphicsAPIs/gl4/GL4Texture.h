#pragma once

#include "../../renderer/graphicsAPI/Texture.h"
#include "GL4Includes.h"

#include <string>

namespace Lag
{
	class GL4Texture : public Texture
	{
	public:
		inline GLuint getHandle() const { return handle; }
		GLenum getGLType() const;

	private:
		friend class GL4TextureManager;
		GL4Texture(const std::string &imageName, const TextureData &data);
		GL4Texture(const std::vector<std::string> &imageNames, const TextureData &data);
		GL4Texture(const ImageData &imageData, const TextureData &textureData);

		GLuint handle;

		virtual bool loadImplementation() override;
		virtual void unloadImplementation() override;

		void initStorage() const;
		void sendData() const;
		void setParameters() const;
		
		GLenum getPixelDataFormatGL() const;
		GLenum getPixelDataTypeGL() const;
		GLenum getPixelDataSizedFormatGL() const;
		GLint getFilteringModeGL(TextureFilteringMode mode) const;
		GLint getWrappingModeGL(TexturewWrappingMode mode) const;
	};
}