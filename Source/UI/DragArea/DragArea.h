//
// Created by Seth Climenhaga on 2022-05-20.
//

#pragma once
#include <JuceHeader.h>


class DraggableNodeEditor;

class DragArea : public juce::Component{

public:
    DragArea();
    ~DragArea() override;

    void paint (juce::Graphics& g) override;
    void addNode(DraggableNodeEditor* node);
    void resized() override;

    void reDrawNode(unsigned int id,int x,int y);

    std::vector<std::unique_ptr<DraggableNodeEditor>> draggableNodes;
};
