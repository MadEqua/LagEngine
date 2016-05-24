#pragma once

namespace Lag
{
	class VertexDescription;
	class VertexBuffer;
	
	/*
	* This represents the Input Assembler input objects of the rendering pipeline. 
	* (VAO on OpenGL, Input-Layout Object on D3D)
	* Concrete GraphicsAPIs will have concrete implementations.
	*/
	class InputDescription
	{
	public:
		virtual void bind() = 0;

	protected:
		//Can only be created by InputDescriptionManager
		friend class InputDescriptionManager;
		InputDescription(const VertexDescription &vertexDescription, const VertexBuffer &vertexBuffer);
		virtual ~InputDescription();


		const VertexDescription &vertexDescription;
		
		//Can only reference one buffer for now (much like VertexData)
		const VertexBuffer &vertexBuffer;
	};
}