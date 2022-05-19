/*
  ==============================================================================

    DraggableNode.h
    Created: 16 May 2021 9:20:05pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include <utility>
#include "../DraggableNodeIdentifiers.h"
#include "../Backend/CurveManager.h"

//==============================================================================
class DraggableNodeEditor  : public juce::Component
{
public:
    DraggableNodeEditor(int identifier, CurveManager cM) : curveManager(std::move(cM))
    {
        this->id = identifier;
    }

   ~DraggableNodeEditor() override= default;

    void paint (juce::Graphics& g)
    {
        //g.fillAll (juce::Colours::blue);   // clear the background

        g.setColour (juce::Colours::orange);
        g.fillEllipse(0, 0, 10, 10);
        g.drawEllipse(0, 0, 10, 10, 1);
    }


    void mouseDrag (const juce::MouseEvent& e) override
    {
        curveManager.moveNode(id, e.position.getX(),  e.position.getY());
    }

private:
    int id;
    CurveManager curveManager;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DraggableNodeEditor)
};
