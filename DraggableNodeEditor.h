/*
  ==============================================================================

    DraggableNode.h
    Created: 16 May 2021 9:20:05pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class DraggableNode  : public juce::Component
{
public:
    DraggableNode();
    ~DraggableNode() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DraggableNode)
};
