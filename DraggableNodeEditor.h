/*
  ==============================================================================

    DraggableNode.h
    Created: 16 May 2021 9:20:05pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DraggableNode.h"
#include "DraggableNodeIdentifiers.h"

//==============================================================================
/*
*/
class DraggableNodeEditor  : public juce::Component
{
public:
    DraggableNodeEditor(juce::Rectangle<int> bounds, juce::ValueTree nodes, int id);
    ~DraggableNodeEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& e) override;
    void mouseDrag (const juce::MouseEvent& e) override;

private:
    
    juce::ComponentDragger myDragger;
    juce::ComponentBoundsConstrainer* containerBounds;
    
    int id;
    juce::ValueTree nodeTree;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DraggableNodeEditor)
};
