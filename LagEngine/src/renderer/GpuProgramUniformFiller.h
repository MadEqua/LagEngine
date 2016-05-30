#pragma once

#include <vector>
#include "../Types.h"

namespace Lag
{
	class GpuProgram;
	class PointLight;
	class DirectionalLight;
	class Viewport;
	enum GpuProgramUniformSemantic;
	
	/*
	* This class fills the uniforms with Engine-defined semantics. (Lights and Camera/Viewport related data)
	* It keeps track of the last update on each GpuProgram to avoid resending similar values.
	* The Renderer calls the appropriate methods to only send new data when it's actually needed.
	*/
	class GpuProgramUniformFiller
	{
	public:
		GpuProgramUniformFiller();
		~GpuProgramUniformFiller();

		//This exists for the case where a frame finishes with the same GpuProgram than the next one will start
		//onGpuProgramBind will not be called in that case, but some uniforms may need to be updated.
		void onFrameStart(const GpuProgram *gpuProgram, uint64 frameNumber, const Viewport *viewport);
		
		void onGpuProgramBind(const GpuProgram *gpuProgram, uint64 frameNumber, const Viewport *viewport);
		void onViewportBind(const GpuProgram *gpuProgram, const Viewport *viewport);

	private:
		void updateLightUniforms(const GpuProgram &gpuProgram);
		void updateViewportUniforms(const GpuProgram &gpuProgram, const Viewport &viewport);

		bool programContainsUniform(const GpuProgram &gpuProgram,
			GpuProgramUniformSemantic semantic);

		void setUniform(const GpuProgram &gpuProgram,
			GpuProgramUniformSemantic semantic, const void* value, uint32 arraySize = 1);

		struct GpuProgramData
		{
			GpuProgramData() : lastUpdatedFrame(0),
				lastUpdatedViewport(nullptr) {}
			
			uint64 lastUpdatedFrame;
			const Viewport *lastUpdatedViewport;
			//last updated renderable?
		};

		std::vector<std::pair<const GpuProgram*, GpuProgramData>> gpuPrograms;

		GpuProgramData& getGpuProgramData(const GpuProgram* gpuProgram);

		//References to useful data to fill Uniforms
		const std::vector<PointLight*> &pointLights;
		const std::vector<DirectionalLight*> &directionalLights;


		struct PairComparator
		{
			bool operator() (const std::pair<const GpuProgram*, GpuProgramData> &p)
			{
				return p.first == program;
			}

			const GpuProgram* program;
		};
		PairComparator comparator;
	};
}