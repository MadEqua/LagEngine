#pragma once

namespace Lag {
    class IFrameListener {
    public:
        virtual ~IFrameListener() = default;

        //At the start of every new frame. Good for game logic updates.
        virtual void onFrameStart(float timePassed) = 0;

        //All render targets had their render commands issued. Called before the buffer swap request.
        //Good point to use the CPU, while the render commands are processed by the GPU.
        virtual void onFrameRenderingQueued(float timePassed) = 0;

        //At the end of every frame. After buffer swaps.
        virtual void onFrameEnd(float timePassed) = 0;
    };
}