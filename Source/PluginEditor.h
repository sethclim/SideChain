/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/DynamicCurve/DynamicCurveEditor.h"

//==============================================================================
struct AtomicLabel
    : juce::Component
    , juce::Timer
{
    explicit AtomicLabel(std::atomic<double>& valueToUse) : value(valueToUse)
    {
        startTimerHz(60);
        addAndMakeVisible(label);
    }

    void resized() override { label.setBounds(getLocalBounds()); }

    void timerCallback() override
    {
        label.setText(juce::String(value.load()), juce::dontSendNotification);
    }

    juce::Label label;
    std::atomic<double>& value;
};

class SideChainAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit SideChainAudioProcessorEditor (SideChainAudioProcessor&);
    ~SideChainAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SideChainAudioProcessor& audioProcessor;
    
public:
    DynamicCurveEditor DynamicCurveEditor;
    
    AtomicLabel volLabel;
    AtomicLabel relLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SideChainAudioProcessorEditor)
};
