//
// Created by Seth Climenhaga on 2022-05-20.
//

#pragma once
#include <JuceHeader.h>
#include "../../Backend/CurveManager.h"


class DraggableNodeEditor;

class DragArea : public juce::Component{

public:
    DragArea(juce::ValueTree  tree);
    ~DragArea() override;

    void paint (juce::Graphics& g) override;
    void addNode(int identifier,int x, int y, CurveManager& curveManager);
    void resized() override;

    void reDrawNode(unsigned int id, juce::Point<float> position);

    void mouseDrag (const juce::MouseEvent& e) override;

    //std::vector<std::unique_ptr<DraggableNodeEditor>> draggableNodes;

private:
    juce::ValueTree nodes;
};
