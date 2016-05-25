#pragma once

#include "../../renderer/graphicsAPI/InputDescription.h"
#include "GL4Includes.h"

namespace Lag
{
	enum VertexAttributeType;
	
	class GL4InputDescription : public InputDescription
	{
	public:
		GL4InputDescription(const VertexDescription &vertexDescription, const GpuBuffer &vertexBuffer);
		virtual ~GL4InputDescription();

		virtual void bind() const override;
	
	private:
		void setupVertexAttributtes() const;
		GLuint handle;

		GLenum convertAttributeTypeToGL(VertexAttributeType type) const;
	};
}