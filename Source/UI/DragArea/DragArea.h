//
// Created by Seth Climenhaga on 2022-05-20.
//

#pragma once
#include <JuceHeader.h>
#include "../../Backend/CurveManager.h"

class DraggableNodeEditor;

class DragArea : public juce::Component
{

public:
    DragArea(const juce::ValueTree &tree, CurveManager &dynCurM);
    ~DragArea() override;

    void paint(juce::Graphics &g) override;
    void resized() override;

    void reDraw();

    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &e) override;
    bool keyPressed(const KeyPress &key) override;

    juce::Point<float> scaleToCoord(juce::Point<float> position);

private:
    juce::ValueTree nodes;
    CurveManager &curveManager;
    int selectedNodeId = -1;
    bool addMode = false;
};
