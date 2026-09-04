#include "PresetBar.h"

namespace ui
{

    PresetBar::PresetBar() : button_("No Preset Selected"), nextButton_("^"), previousButton_("v")
    {
        addChild(button_);
        addChild(nextButton_);
        addChild(previousButton_);

        button_.onToggle() += [this](visage::Button *, bool)
        { showMenu(); };

        nextButton_.onToggle() += [this](visage::Button *, bool)
        {
            if (onNextPreset_)
                onNextPreset_();
        };

        previousButton_.onToggle() += [this](visage::Button *, bool)
        {
            if (onPreviousPreset_)
                onPreviousPreset_();
        };
    }

    PresetBar::~PresetBar() = default;

    void PresetBar::setCurrentPresetName(const std::string &name)
    {
        button_.setText(name.empty() ? "No Preset Selected" : name);
    }

    void PresetBar::setPresetNames(std::vector<std::string> names)
    {
        presetNames_ = std::move(names);
    }

    void PresetBar::setOnPresetSelected(std::function<void(const std::string &)> callback)
    {
        onPresetSelected_ = std::move(callback);
    }

    void PresetBar::setOnNextPreset(std::function<void()> callback)
    {
        onNextPreset_ = std::move(callback);
    }

    void PresetBar::setOnPreviousPreset(std::function<void()> callback)
    {
        onPreviousPreset_ = std::move(callback);
    }

    void PresetBar::resized()
    {
        float nameWidth = 160.0f;
        float nameHeight = height() * 0.6f;
        float arrowWidth = 18.0f;
        float groupSpacing = 8.0f;
        float arrowGap = 3.0f;

        float groupWidth = nameWidth + groupSpacing + arrowWidth;
        float groupX = width() * 0.5f - groupWidth * 0.5f;
        float groupY = height() * 0.5f - nameHeight * 0.5f;

        button_.setBounds(groupX, groupY, nameWidth, nameHeight);

        float arrowX = groupX + nameWidth + groupSpacing;
        float arrowHeight = (nameHeight - arrowGap) * 0.5f;
        nextButton_.setBounds(arrowX, groupY, arrowWidth, arrowHeight);
        previousButton_.setBounds(arrowX, groupY + arrowHeight + arrowGap, arrowWidth, arrowHeight);
    }

    void PresetBar::showMenu()
    {
        if (presetNames_.empty())
            return;

        visage::PopupMenu menu;
        for (int i = 0; i < (int)presetNames_.size(); ++i)
            menu.addOption(i, presetNames_[i]);

        menu.onSelection() += [this](int id)
        {
            if (id >= 0 && id < (int)presetNames_.size() && onPresetSelected_)
                onPresetSelected_(presetNames_[id]);
        };

        menu.show(&button_);
    }

}
