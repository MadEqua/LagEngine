#pragma once

#include "../../renderer/graphicsAPI/InputDescription.h"
#include "GL4Includes.h"

namespace Lag
{
	enum VertexAttributeType;
	
	class GL4InputDescription : public InputDescription
	{
	public:		
		virtual bool loadImplementation() override;
		virtual void unloadImplementation() override;

		inline GLuint getHandle() const { return handle; }

	private:
		friend class GL4InputDescriptionBuilder;
		GL4InputDescription(const VertexDescription &vertexDescription, const Handle<GpuBuffer> vertexBuffer);

		void setupVertexAttributtes() const;
		GLenum convertAttributeTypeToGL(VertexAttributeType type) const;

		GLuint handle;
	};
}