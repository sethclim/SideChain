#pragma once

#include <visage/app.h>
#include <visage/widgets.h>

#include <functional>
#include <string>

// Everything under UI/ renders with visage and must stay free of JUCE
// includes, so this view can eventually be reused or tested outside the
// plugin. The plugin host (Source/PluginEditor) only ever talks to it
// through this interface plus a native window handle.
namespace ui {

class VisageMainView {
public:
    VisageMainView();
    ~VisageMainView();

    // Embeds the visage window as a native child of parentNativeHandle
    // (HWND on Windows), sized to width x height native pixels.
    void embed(void* parentNativeHandle, int width, int height);

    // Call when the host's bounds change while embedded.
    void resize(int width, int height);

    // Tears down the native child window. Safe to call even if never embedded.
    void remove();

    // The division button is display-only from this class's point of view:
    // JUCE owns the real parameter value and pushes label updates in here.
    void setDivisionLabel(const std::string& label);

    // Fired when the user clicks the division button. The host decides what
    // "next" means for the underlying JUCE parameter.
    void setDivisionClickedCallback(std::function<void()> callback);

private:
    void layoutChildren(int width, int height);

    visage::ApplicationWindow window_;
    visage::UiButton divisionButton_;
    std::function<void()> divisionClicked_;
};

}
