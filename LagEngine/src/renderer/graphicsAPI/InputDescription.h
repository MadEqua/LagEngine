#pragma once

namespace Lag
{
	class VertexDescription;
	class VertexBuffer;
	
	/*
	* This represents the Input Assembler entry-point objects of the rendering pipeline. 
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


		//How to interpret the vertices
		const VertexDescription &vertexDescription;
		
		// Where to get the vertices from
		//TODO: maybe add a Bindings class (id -> buffer) to allow getting different attributes from different buffers. (add a binding id on VertexAttribute also)
		const VertexBuffer &vertexBuffer;
	};
}