#pragma once

#include "../../Types.h"
#include "../../core/ManagedObject.h"

namespace Lag
{
	class VertexDescription;
	class GpuBuffer;
	
	/*
	* This represents the Input Assembler entry-point objects of the rendering pipeline. 
	* (VAO on OpenGL, Input-Layout Object on D3D)
	* Concrete GraphicsAPIs will have concrete implementations.
	*/
	class InputDescription : public ManagedObject
	{
	protected:
		InputDescription(const VertexDescription &vertexDescription, const GpuBuffer &vertexBuffer);

		//How to interpret the vertices
		const VertexDescription &vertexDescription;
		
		// Where to get the vertices from
		//TODO: maybe add a Bindings class (id -> buffer) to allow getting different attributes from different buffers. (add a binding id on VertexAttribute also)
		const GpuBuffer &vertexBuffer;
	};
}