#pragma once

#include <vector>
#include <unordered_map>
#include "../Types.h"

namespace Lag
{
	class GpuProgram;
	class PointLight;
	class DirectionalLight;
	class Viewport;
	class Texture;
	enum GpuProgramUniformSemantic;
	class TextureBindings;
	
	/*
	* This class fills the uniforms with Engine-defined semantics. (Lights and Camera/Viewport related data)
	* The Renderer calls the appropriate methods to only send new data when it's actually needed.
	*/
	class GpuProgramUniformFiller
	{
	public:
		GpuProgramUniformFiller();
		~GpuProgramUniformFiller();

		//This exists for the case where a frame finishes with the same GpuProgram than the next one will start
		//onGpuProgramBind will not be called in that case, but some uniforms may need to be updated.
		void onFrameStart(const GpuProgram *gpuProgram, const Viewport *viewport, const TextureBindings &textureBindings);
		
		void onGpuProgramBind(const GpuProgram *gpuProgram, const Viewport *viewport, const TextureBindings &textureBindings);
		void onViewportBind(const GpuProgram *gpuProgram, const Viewport *viewport);
		void onTextureBind(const GpuProgram *gpuProgram, const Texture* texture, uint8 unit);

	private:
		void updateLightUniforms(const GpuProgram &gpuProgram);
		void updateViewportUniforms(const GpuProgram &gpuProgram, const Viewport &viewport);
		void updateTextureUniforms(const GpuProgram &gpuProgram, const Texture &texture, uint8 unit);

		bool programContainsUniform(const GpuProgram &gpuProgram,
			GpuProgramUniformSemantic semantic);

		void setUniform(const GpuProgram &gpuProgram,
			GpuProgramUniformSemantic semantic, const void* value, uint32 arraySize = 1);

		//References to useful data to fill Uniforms
		const std::vector<PointLight*> &pointLights;
		const std::vector<DirectionalLight*> &directionalLights;
	};
}