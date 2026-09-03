#pragma once

#include <visage/app.h>
#include <visage/widgets.h>

#include "DragView.h"
#include "MeterView.h"
#include "PresetBar.h"

#include <functional>
#include <string>
#include <utility>
#include <vector>

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

        // The division button is display-only from this class's point of view:
        // JUCE owns the real parameter value and pushes label updates in here.
        void setDivisionLabel(const std::string &label);

        // Fired when the user clicks the division button. The host decides what
        // "next" means for the underlying JUCE parameter.
        void setDivisionClickedCallback(std::function<void()> callback);

        // Curve drag area: JUCE owns the real node data (CurveManager) and pushes
        // normalized (0-1, 0-1) points in here whenever they change.
        void setCurvePoints(std::vector<std::pair<float, float>> normalizedPoints);

        // Fired while the user drags a node: index into the vector last passed to
        // setCurvePoints(), plus its new normalized position.
        void setOnCurveNodeDragged(std::function<void(int index, float x, float y)> callback);

        // Fired when the user requests a new node at a normalized position.
        void setOnCurvePointAdded(std::function<void(float x, float y)> callback);

        // Preset bar: JUCE owns the real preset list/state (PresetManager) and
        // pushes the current name and available names in here.
        void setCurrentPresetName(const std::string &name);
        void setPresetNames(std::vector<std::string> names);

        // Fired when the user picks a preset from the bar's dropdown.
        void setOnPresetSelected(std::function<void(const std::string &name)> callback);

        // Fired by the preset bar's up/down arrows.
        void setOnNextPreset(std::function<void()> callback);
        void setOnPreviousPreset(std::function<void()> callback);

        // Level meters: the host supplies a poll function per channel (e.g.
        // reaching into the processor's atomic RMS value), matching
        // Source/UI/Meters/Meters.h's VerticalMeter constructor.
        void setLeftMeterSupplier(std::function<float()> valueSupplier);
        void setRightMeterSupplier(std::function<float()> valueSupplier);

    private:
        void layoutChildren(int width, int height);

        visage::ApplicationWindow window_;
        visage::UiButton divisionButton_;
        DragView dragView_;
        PresetBar presetBar_;
        MeterView leftMeter_;
        MeterView rightMeter_;
        std::function<void()> divisionClicked_;
    };

}
