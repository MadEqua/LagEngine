#pragma once

namespace Lag {
    class RenderWindow;

    class IWindowListener {
    public:
        virtual void onMove(RenderWindow &notifier, uint32 x, uint32 y) = 0;
        virtual void onClose(RenderWindow &notifier) = 0;
        virtual void onFocusChange(RenderWindow &notifier, bool focused) = 0;
    };
}