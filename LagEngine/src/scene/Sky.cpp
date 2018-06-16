#include "Sky.h"

#include "Renderer.h"
#include "Material.h"
#include "GpuBufferManager.h"
#include "VertexDescription.h"
#include "InputDescriptionManager.h"
#include "MaterialManager.h"

using namespace Lag;

Sky::Sky(const std::string &materialName) :
        nor(1.0f),
        model(1.0f) {
    material = Root::getInstance().getMaterialManager().get(materialName);

    if (!material.isValid()) {
        //TODO
    }

    VertexDescription vxDesc;
    vxDesc.addAttribute(VertexAttributeSemantic::POSITION, 3, VertexAttributeType::FLOAT);

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
    auto &bufferBuilder = dynamic_cast<GpuBufferBuilder &>(gpuBufferManager.getBuilder());
    bufferBuilder.contents = GpuBufferContents::VERTICES;
    bufferBuilder.flags = static_cast<uint32>(GpuBufferUsage::DYNAMIC);
    bufferBuilder.itemCount = VERTEX_COUNT;
    bufferBuilder.itemSizeBytes = vxDesc.getByteSize();
    bufferBuilder.useMirrorBuffer = false;
    Handle<GpuBuffer> vertexBuffer = gpuBufferManager.get();

    bufferBuilder.contents = GpuBufferContents::INDICES;
    bufferBuilder.flags = static_cast<uint32>(GpuBufferUsage::DYNAMIC);
    bufferBuilder.itemCount = INDEX_COUNT;
    bufferBuilder.itemSizeBytes = sizeof(uint8);
    bufferBuilder.useMirrorBuffer = false;
    Handle<GpuBuffer> indexBuffer = gpuBufferManager.get();

    vertexBuffer->lock();
    vertexBuffer->write(0, vertexBuffer->getSize(), reinterpret_cast<byte *>(vertices));
    vertexBuffer->unlock();

    indexBuffer->lock();
    indexBuffer->write(0, indexBuffer->getSize(), reinterpret_cast<byte *>(indices));
    indexBuffer->unlock();

    if (!vertexBuffer.isValid() || !indexBuffer.isValid()) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
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
    indexData.indexType = IndexType::UINT8;
}

void Sky::addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) {
    if (renderTarget.getRenderPhase() == RenderPhase::COLOR) {
        RenderOperation &ro = renderQueue.addRenderOperation();
        ro.renderMode = RenderMode::TRIANGLES;
        ro.renderTarget = &renderTarget;
        ro.vertexData = &vertexData;
        ro.indexData = &indexData;
        ro.material = material.get();
        ro.renderable = this;
        ro.viewport = &viewport;
        ro.passId = 0;
    }
}

void Sky::render(Renderer &renderer, RenderOperation &renderOperation) {
    renderer.getUniformFiller().onRenderableRender(renderOperation.material->getGpuProgram(),
                                                   model, nor, *renderOperation.viewport);

    renderer.renderIndexed(renderOperation.renderMode, *renderOperation.vertexData, *renderOperation.indexData);
}