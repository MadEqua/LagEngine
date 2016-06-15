#pragma once

#include "../renderer/IRenderable.h"
#include <string>
#include "../renderer/VertexData.h"
#include "../renderer/IndexData.h"

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

namespace Lag
{
	class Material;
	
	//TODO: use a code (non XML) generated material (texture is the only input)
	class Sky : public IRenderable
	{
	public:
		Sky(const std::string &materialName);
		//Sky(const std::string &textureName);
		~Sky();

		virtual void addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) override;
		virtual void render(Renderer &renderer, RenderOperation &renderOperation) override;

	private:
		Material *material;
		VertexData vertexData;
		IndexData indexData;

		glm::mat3 nor;
		glm::mat4 model;
	};
}