#pragma once

#include <visage/app.h>

// Everything under UI/ renders with visage and must stay free of JUCE
// includes, so this view can eventually be reused or tested outside the
// plugin. The plugin host (Source/PluginEditor) only ever talks to it
// through this interface plus a native window handle.
namespace ui
{

    class VisageMainView
    {
    public:
        VisageMainView();
        ~VisageMainView();

        // Embeds the visage window as a native child of parentNativeHandle
        // (HWND on Windows), sized to width x height native pixels.
        void embed(void *parentNativeHandle, int width, int height);

        // Call when the host's bounds change while embedded.
        void resize(int width, int height);

        // Tears down the native child window. Safe to call even if never embedded.
        void remove();

    private:
        visage::ApplicationWindow window_;
    };

}
