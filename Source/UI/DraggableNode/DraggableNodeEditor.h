/*
  ==============================================================================

    DraggableNode.h
    Created: 16 May 2021 9:20:05pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#pragma once


class DragArea;
class CurveManager;

//==============================================================================
class DraggableNodeEditor  : public juce::Component
{
public:
    DraggableNodeEditor(int identifier, int x, int y, CurveManager& cM, DragArea& dA );
   ~DraggableNodeEditor() override;

    void paint (juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& event) override;
    //void mouseDrag (const juce::MouseEvent& e) override;

    int id, startX, startY;
private:
    CurveManager& curveManager;
    DragArea& dragArea;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DraggableNodeEditor)
};
