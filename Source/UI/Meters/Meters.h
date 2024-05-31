#pragma once
#include <JuceHeader.h>

class VerticalMeter : public Component, public Timer
{
public:
    VerticalMeter(std::function<float()> &&valueFunction) : valueSupplier(std::move(valueFunction))
    {
        startTimerHz(24);
    }

    void paint(Graphics &g) override
    {
        const auto level = valueSupplier();
        auto bounds = getLocalBounds().toFloat();
        g.setColour(juce::Colours::black);
        g.fillRect(bounds);
        g.setGradientFill(gradient);
        const auto scaledY = jmap<float>(level, -60.0f, 6.f, 0, static_cast<float>(getHeight()));
        g.fillRect(bounds.removeFromBottom(scaledY));
    }

    void resized() override
    {
        auto bounds = getLocalBounds().toFloat();
        gradient = ColourGradient{
            juce::Colours::green,
            bounds.getTopLeft(),
            juce::Colours::red,
            bounds.getBottomLeft(),
            false,
        };

        gradient.addColour(0.5, juce::Colours::yellow);
    }

    void timerCallback() override
    {
        repaint();
    }

private:
    std::function<float()> valueSupplier;
    ColourGradient gradient{};
};
