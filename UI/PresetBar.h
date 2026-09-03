#pragma once

#include <visage/ui.h>
#include <visage/widgets.h>

#include <functional>
#include <string>
#include <vector>

namespace ui
{

    // Top bar showing the current preset name as a small centered button,
    // with a tight up/down arrow stack to its right for next/previous.
    // Clicking the name opens a dropdown (visage::PopupMenu) listing every
    // preset name last passed to setPresetNames(). JUCE (via
    // Service::PresetManager) owns the real list and the currently-loaded
    // preset - this class only displays what it's told and reports what the
    // user picked or requested.
    class PresetBar : public visage::Frame
    {
    public:
        PresetBar();
        ~PresetBar() override;

        void setCurrentPresetName(const std::string &name);
        void setPresetNames(std::vector<std::string> names);
        void setOnPresetSelected(std::function<void(const std::string &name)> callback);

        // Fired by the up/down arrows.
        void setOnNextPreset(std::function<void()> callback);
        void setOnPreviousPreset(std::function<void()> callback);

        void resized() override;

    private:
        void showMenu();

        visage::UiButton button_;
        visage::UiButton nextButton_;
        visage::UiButton previousButton_;
        std::vector<std::string> presetNames_;
        std::function<void(const std::string &)> onPresetSelected_;
        std::function<void()> onNextPreset_;
        std::function<void()> onPreviousPreset_;
    };

}
