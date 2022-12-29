//
// Created by Seth Climenhaga on 2022-05-21.
//
#include "DragArea.h"

#include <utility>
#include "../DraggableNode/DraggableNodeEditor.h"

DragArea::DragArea(juce::ValueTree tree) : nodes(std::move(tree)) { }

DragArea::~DragArea() = default;

void DragArea::paint (juce::Graphics& g)
{
    for(const auto& child : nodes)
    {
        const auto& x = child.getProperty(DraggableNodeIdentifiers::posX);
        const auto& y = child.getProperty(DraggableNodeIdentifiers::posY);

        g.setColour (juce::Colours::orange);
        g.fillEllipse(x,  y, 10, 10);
        g.drawEllipse(x,  y,  10,  10,1);
    }
}

void DragArea::addNode(int identifier, int x, int y, CurveManager& curveManager){
    //std::cout<< "addNode: "<< identifier << " X  " << (int)x <<" y " << (int)y << std::endl;
//    auto node = std::make_unique<DraggableNodeEditor>(identifier, x, y, curveManager, *this);
//    addAndMakeVisible(*node);
//    node->setBounds(node->startX, node->startY, 10, 10);
//    draggableNodes.emplace_back(std::move(node));
}

void DragArea::reDrawNode(unsigned int id, juce::Point<float> position){
    //std::cout<< "Edit: "<< id << " X  " << (int)x <<" y " << (int)y << std::endl;
//    draggableNodes[id]->setBounds((int)position.getX(), (int)position.getY(), 10, 10);
//    repaint();
}

void DragArea::resized(){
//    for(const auto& child : draggableNodes)
//    {
//        int x = child->startX;
//        int y = child->startY;
//
//        child->setBounds(x ,y, 10, 10);
//    }
}

void DragArea::mouseDrag(const juce::MouseEvent& e)
{

    DBG(typeid(e.eventComponent).name());

    if(strcmp(typeid(e.eventComponent).name(), "DraggableNodeEditor") == 0){
        float x = static_cast<float>(e.getPosition().getX());
        float y = static_cast<float>(e.getPosition().getY());
        std::cout << "THE mouseDrag: X " << e.getPosition().getX() << " Y " << e.getPosition().getY() <<std::endl;

        //auto d = dynamic_cast<DraggableNodeEditor*>(e.eventComponent);
        //curveManager.moveNode(d->id, x, y);
    }
}