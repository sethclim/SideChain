//
// Created by Seth Climenhaga on 2022-05-20.
//

#pragma once
#include <JuceHeader.h>
#include "../../Backend/CurveManager.h"

class DragArea : public juce::Component, ValueTree::Listener
{

public:
    DragArea(juce::AudioProcessorValueTreeState &tree, CurveManager &curveManager);
    ~DragArea() override;

    void paint(juce::Graphics &g) override;
    void resized() override;

    void reDraw();
    void StartListening();

    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &e) override;

    juce::Point<float> scaleToCoord(juce::Point<float> position);

    void valueTreeRedirected(ValueTree &treeWhichHasBeenChanged) override;

private:
    juce::AudioProcessorValueTreeState &apvts;
    CurveManager &m_CurveManager;
    juce::var selectedNodeId = "-1";
};
