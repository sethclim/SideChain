//
// Created by Seth Climenhaga on 2022-05-21.
//
#include "DragArea.h"
#include <utility>

DragArea::DragArea(const juce::ValueTree& tree, CurveManager& dynCurM) : nodes(std::move(tree)), curveManager(dynCurM) { }
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

void DragArea::addNode(int identifier, int x, int y){
    //std::cout<< "addNode: "<< identifier << " X  " << (int)x <<" y " << (int)y << std::endl;
//    auto node = std::make_unique<DraggableNodeEditor>(identifier, x, y, curveManager, *this);
//    addAndMakeVisible(*node);
//    node->setBounds(node->startX, node->startY, 10, 10);
//    draggableNodes.emplace_back(std::move(node));
}

void DragArea::reDrawNode(unsigned int id, juce::Point<float> position){
    repaint();
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

void DragArea::mouseDown(const juce::MouseEvent &event) {
    juce::Point<float> pos = event.position;

    //Check for a selected node and set it
    for(const auto& child : nodes)
    {
        const auto& x = child.getProperty(DraggableNodeIdentifiers::posX);
        const auto& y = child.getProperty(DraggableNodeIdentifiers::posY);

        juce::Point<float> nodePos = juce::Point<float>((float) x, (float) y);
        //Check the distance between the point and where I click if its less than the radius// will work if move the center
        //Currently just a box check
        if(pos.x > nodePos.x && pos.x < nodePos.x + 10 && pos.y > nodePos.y && pos.y < nodePos.y + 10)
        {
            selectedNodeId = child.getProperty(DraggableNodeIdentifiers::id);
            break;
        }
        else
        {
            selectedNodeId = -1;
        }
    }

    if(selectedNodeId == -1)
    {
        curveManager.addNewNode(static_cast<int>(pos.x), static_cast<int>(pos.y));
    }
}

void DragArea::mouseDrag(const juce::MouseEvent& e)
{
    if(selectedNodeId != -1)
        curveManager.moveNode(selectedNodeId, e.position);
}