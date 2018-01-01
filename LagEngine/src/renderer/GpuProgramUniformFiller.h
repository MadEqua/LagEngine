#pragma once

#include <vector>
#include <unordered_map>
#include "../Types.h"
#include <glm/mat4x4.hpp>

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
	* This class fills the uniforms with Engine-defined semantics.
	* The Renderer calls the appropriate methods to only send new data when it's actually needed.
	* A Renderable can also rely on this class to quickly fill all the needed Uniforms.
	*/
	class GpuProgramUniformFiller
	{
	public:
		GpuProgramUniformFiller();

		void onGpuProgramBind(const GpuProgram *gpuProgram, const Viewport *viewport, const TextureBindings &textureBindings);
		void onViewportBind(const GpuProgram *gpuProgram, const Viewport *viewport);
		void onTextureBind(const GpuProgram *gpuProgram, const Texture* texture, uint8 unit);

		void onRenderableRender(const GpuProgram &gpuProgram, const glm::mat4 &modelMatrix, 
			const glm::mat3 &normalMatrix, const Viewport &viewport);

	private:
		void updateLightUniforms(const GpuProgram &gpuProgram);
		void updateViewportUniforms(const GpuProgram &gpuProgram, const Viewport &viewport);
		void updateTextureUniforms(const GpuProgram &gpuProgram, const Texture &texture, uint8 unit);

		bool programContainsUniform(const GpuProgram &gpuProgram,
			GpuProgramUniformSemantic semantic);

		void setUniformIfPresent(const GpuProgram &gpuProgram,
			GpuProgramUniformSemantic semantic, const void* value, uint32 arraySize = 1);

		//References to useful data to fill Uniforms
		const std::vector<PointLight*> &pointLights;
		const std::vector<DirectionalLight*> &directionalLights;

		const GpuProgram *frameEndGpuProgram;
	};
}