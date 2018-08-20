#include "AxisGizmo.h"

#include "Root.h"
#include "Scene.h"
#include "GpuBufferManager.h"
#include "VertexDescription.h"
#include "InputDescriptionManager.h"
#include "GpuProgramUniformFiller.h"
#include "Renderer.h"
#include "MaterialManager.h"

using namespace Lag;

//TODO: reutilize the same buffers/mesh between all the gizmos
AxisGizmo::AxisGizmo() {

    material = Root::getInstance().getMaterialManager().get("axisGizmoMaterial");

    if (!material.isValid()) {
        //TODO
    }

    const int VERTEX_COUNT = 4;
    const int INDEX_COUNT = 6;

    VertexDescription vxDesc;
    vxDesc.addAttribute(VertexAttributeSemantic::POSITION, 3, VertexAttributeType::FLOAT);

    GpuBufferManager &gpuBufferManager = Root::getInstance().getGpuBufferManager();
    auto &bufferBuilder = dynamic_cast<GpuBufferBuilder &>(gpuBufferManager.getBuilder());
    bufferBuilder.contents = GpuBufferContents::VERTICES;
    bufferBuilder.flags = static_cast<uint32>(GpuBufferUsage::DYNAMIC);
    bufferBuilder.itemCount = VERTEX_COUNT;
    bufferBuilder.itemSizeBytes = vxDesc.getVertexByteSize();
    bufferBuilder.useMirrorBuffer = false;
    Handle<GpuBuffer> vertexBuffer = gpuBufferManager.get();

    bufferBuilder.contents = GpuBufferContents::INDICES;
    bufferBuilder.flags = static_cast<uint32>(GpuBufferUsage::DYNAMIC);
    bufferBuilder.itemCount = INDEX_COUNT;
    bufferBuilder.itemSizeBytes = sizeof(uint8);
    bufferBuilder.useMirrorBuffer = false;
    Handle<GpuBuffer> indexBuffer = gpuBufferManager.get();

    float vertices[] = {
            0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
    };

    uint8 indices[] = {
            0, 1,
            0, 2,
            0, 3
    };

    vertexBuffer->lock();
    vertexBuffer->write(0, vertexBuffer->getSize(), reinterpret_cast<byte *>(vertices));
    vertexBuffer->unlock();

    indexBuffer->lock();
    indexBuffer->write(0, indexBuffer->getSize(), reinterpret_cast<byte *>(indices));
    indexBuffer->unlock();

    vertexData.vertexStart = 0;
    vertexData.vertexCount = VERTEX_COUNT;

    InputDescriptionManager &inputDescriptionManager = Root::getInstance().getInputDescriptionManager();
    vertexData.inputDescription = inputDescriptionManager.get(vxDesc, vertexBuffer);

    indexData.indexStart = 0;
    indexData.indexCount = INDEX_COUNT;
    indexData.indexBuffer = indexBuffer;
    indexData.indexType = IndexType::UINT8;
}


void AxisGizmo::addToRenderQueue(RenderQueue &renderQueue, Viewport &viewport, RenderTarget &renderTarget) {
    if (renderTarget.getRenderPhase() == RenderPhase::COLOR) {
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

void AxisGizmo::render(Renderer &renderer, RenderOperation &renderOperation) {
    renderer.getUniformFiller().onRenderableRender(renderOperation.material->getGpuProgram(),
                                                   *renderOperation.viewport,
                                                   getLocalToWorldTransform());

    //renderer.renderIndexed(renderOperation.material->getRenderMode(), *renderOperation.vertexData, *renderOperation.indexData);
    renderer.renderIndexed(RenderMode::LINES, *renderOperation.vertexData, *renderOperation.indexData);
}