#pragma once
#include <JuceHeader.h>

class ControlPoint : public juce::Component
{
public:
    ControlPoint();
    ~ControlPoint();

    void paint(juce::Graphics &g) override;
    void resized() override;
};