#include "Sky.h"

#include "../renderer/Renderer.h"
#include "../renderer/RenderQueue.h"
#include "../Root.h"
#include "../renderer/Material.h"
#include "../renderer/graphicsAPI/GpuBufferManager.h"
#include "../renderer/graphicsAPI/GpuBuffer.h"
#include "../renderer/VertexDescription.h"
#include "../renderer/graphicsAPI/InputDescriptionManager.h"
#include "../resources/MaterialManager.h"
#include "../Types.h"
#include "../renderer/RenderTarget.h"

using namespace Lag;

Sky::Sky(const std::string &materialName)
{
	material = Root::getInstance().getMaterialManager().get(materialName);
	
	if (!material.isValid())
	{
		//TODO
	}

	VertexDescription vxDesc;
	vxDesc.addAttribute(LAG_VX_ATTR_SEMANTIC_POSITION, 3, LAG_VX_ATTR_TYPE_FLOAT);

	const int VERTEX_COUNT = 8;
	const int INDEX_COUNT = 12 * 3;
	const float v = 1.0f;
	float vertices[] =
	{
		-v, v, v,
		-v, -v, v,
		v, -v, v,
		v, v, v,
		-v, v, -v,
		-v, -v, -v,
		v, -v, -v,
		v, v, -v
	};

	uint8 indices[INDEX_COUNT] =
	{
		2, 1, 0,
		0, 3, 2,
		4, 5, 6,
		6, 7, 4,
		6, 2, 7,
		2, 3, 7,
		4, 1, 5,
		4, 0, 1,
		5, 1, 2,
		5, 2, 6,
		4, 3, 0,
		4, 7, 3
	};

	GpuBufferManager &gpuBufferManager = Root::getInstance().getGpuBufferManager();
	GpuBufferBuilder &bufferBuilder = static_cast<GpuBufferBuilder&>(gpuBufferManager.getBuilder());
	bufferBuilder.contents = LAG_GPU_BUFFER_CONTENTS_VERTICES;
	bufferBuilder.flags = LAG_GPU_BUFFER_USAGE_DYNAMIC;
	bufferBuilder.itemCount = VERTEX_COUNT;
	bufferBuilder.itemSizeBytes = vxDesc.getByteSize();
	bufferBuilder.useMirrorBuffer = false;
	Handle<GpuBuffer> vertexBuffer = gpuBufferManager.get();

	bufferBuilder.contents = LAG_GPU_BUFFER_CONTENTS_INDICES;
	bufferBuilder.flags = LAG_GPU_BUFFER_USAGE_DYNAMIC;
	bufferBuilder.itemCount = INDEX_COUNT;
	bufferBuilder.itemSizeBytes = sizeof(uint8);
	bufferBuilder.useMirrorBuffer = false;
	Handle<GpuBuffer> indexBuffer = gpuBufferManager.get();

	vertexBuffer->lock();
	vertexBuffer->write(0, vertexBuffer->getSize(), reinterpret_cast<byte*>(vertices));
	vertexBuffer->unlock();

	indexBuffer->lock();
	indexBuffer->write(0, indexBuffer->getSize(), reinterpret_cast<byte*>(indices));
	indexBuffer->unlock();

	if (!vertexBuffer.isValid() || !indexBuffer.isValid())
	{
		LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL,
			"Sky", "Failed to load sky. Failed to build a VertexBuffer or IndexBuffer");
		return;
	}
	
	vertexData.vertexStart = 0;
	vertexData.vertexCount = VERTEX_COUNT;

	InputDescriptionManager &inputDescriptionManager = Root::getInstance().getInputDescriptionManager();
	vertexData.inputDescription = inputDescriptionManager.get(vxDesc, vertexBuffer);

	indexData.indexStart = 0;
	indexData.indexCount = INDEX_COUNT;
	indexData.indexBuffer = indexBuffer;
	indexData.indexType = LAG_IDX_TYPE_UINT8;
}

void Sky::addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget)
{
	if (renderTarget.getRenderPhase() == LAG_RENDER_PHASE_COLOR)
	{
		RenderOperation &ro = renderQueue.addRenderOperation();
		ro.renderTarget = &renderTarget;
		ro.vertexData = &vertexData;
		ro.indexData = &indexData;
		ro.material = material.get();
		ro.renderable = this;
		ro.viewport = &viewport;
		ro.passId = 0;
	}
}

void Sky::render(Renderer &renderer, RenderOperation &renderOperation)
{
	renderer.getUniformFiller().onRenderableRender(renderOperation.material->getGpuProgram(),
		model, nor, *renderOperation.viewport);
	
	renderer.setDepthWritingEnabled(false);
	renderer.renderIndexed(*renderOperation.vertexData, *renderOperation.indexData, 0);
	renderer.setDepthWritingEnabled(true);
}