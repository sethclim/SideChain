//
// Created by Seth Climenhaga on 2022-05-21.
//
#include "DragArea.h"
#include "../DraggableNode/DraggableNodeEditor.h"

DragArea::DragArea()= default;
DragArea::~DragArea()= default;

void DragArea::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    g.setColour (juce::Colours::red);
    g.drawRect (getLocalBounds(), 1);
}

void DragArea::addNode(DraggableNodeEditor* node){
    addAndMakeVisible(*node);
    draggableNodes.emplace_back(node);
    node->setBounds(node->startX ,node->startY, 10, 10);
}

void DragArea::reDrawNode(unsigned int id,int x,int y){
    std::cout<< "Edit: "<< id << " X  " << (int)x <<" y " << (int)y << std::endl;
    draggableNodes[id]->setBounds(x, y, 10, 10);
    repaint();
}

void DragArea::resized(){
    for(const auto& child : draggableNodes)
    {
        int x = child->startX;
        int y = child->startY;


        child->setBounds(x ,y, 10, 10);
    }
}
