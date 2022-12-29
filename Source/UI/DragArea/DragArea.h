//
// Created by Seth Climenhaga on 2022-05-20.
//

#pragma once
#include <JuceHeader.h>
#include "../../Backend/CurveManager.h"


class DraggableNodeEditor;

class DragArea : public juce::Component{

public:
    DragArea(const juce::ValueTree& tree, CurveManager& dynCurM);
    ~DragArea() override;

    void paint (juce::Graphics& g) override;
    void addNode(int identifier,int x, int y);
    void resized() override;

    void reDrawNode(unsigned int id, juce::Point<float> position);

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag (const juce::MouseEvent& e) override;

    //std::vector<std::unique_ptr<DraggableNodeEditor>> draggableNodes;

private:
    juce::ValueTree nodes;
    CurveManager& curveManager;
    int selectedNodeId = -1;
};
