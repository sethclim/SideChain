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
class DraggableNodeEditor  : public juce::Component
{
public:
    DraggableNodeEditor(juce::Rectangle<int> bounds);
    ~DraggableNodeEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& e) override;
    void mouseDrag (const juce::MouseEvent& e) override;

private:
    
    juce::ComponentDragger myDragger;
    juce::ComponentBoundsConstrainer* containerBounds;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DraggableNodeEditor)
};
