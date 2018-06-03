#pragma once

#include "RenderTarget.h"

#include "ObserverPattern.h"
#include "IWindowListener.h"

namespace Lag {

    class IWindowListener;
    class InitializationParameters;

    class RenderWindow : public RenderTarget {
        LAG_GENERATE_OBSERVER_STORAGE(IWindowListener)
        LAG_GENERATE_NOTIFY_METHOD(onMove, IWindowListener, LAG_ARGS(RenderWindow &notifier, uint32 x, uint32 y), LAG_ARGS(notifier, x, y))
        LAG_GENERATE_NOTIFY_METHOD(onClose, IWindowListener, LAG_ARGS(RenderWindow &notifier), LAG_ARGS(notifier))
        LAG_GENERATE_NOTIFY_METHOD(onFocusChange, IWindowListener, LAG_ARGS(RenderWindow &notifier, bool focused), LAG_ARGS(notifier, focused))

    public:
        explicit RenderWindow(const InitializationParameters &parameters);

        //Process messages coming from the OS
        virtual void processEvents() = 0;

        //Useful for camera movement
        virtual void setVirtualCursor(bool value) {}

        virtual bool isVirtualCursorEnabled() { return false; }

    protected:
        const InitializationParameters &initializationParameters;
    };
}