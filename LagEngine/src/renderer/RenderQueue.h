#pragma once

#include <vector>
#include "RenderOperation.h"
#include "Types.h"

namespace Lag {

    struct VertexData;
    struct IndexData;
    class Material;
    class IRenderable;
    class Renderer;
    class IFrameListener;
    enum class RenderPhase : uint8;

    /*
    * Renderer helper class.
    */
    class RenderQueue {
        friend class Renderer;

    public:
        RenderQueue();

        //Returns a RenderOperation to be filled by a Renderable
        RenderOperation &addRenderOperation();

        /*void addRenderOperation(IRenderable &renderable, RenderPhase renderPhase, uint32 passId,
            VertexData &vertexData, IndexData *indexData, Material &material,
            Viewport &viewport);*/

        //which one??
        //void addItem(RenderOperation &renderOperation);

        void dispatchRenderOperations(Renderer &renderer);

        void clear();
        void sort();

        inline bool hasRenderOperations() const { return currentSlot > 0; }

    private:
        uint32 currentSlot;

        //TODO: find out what's better: vector of pointers or actual Values.
        //Pointers are better for sorting, but worse for fast sequential access.
        std::vector<RenderOperation> queue;
    };
}