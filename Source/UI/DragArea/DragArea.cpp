//
// Created by Seth Climenhaga on 2022-05-21.
//
#include "DragArea.h"

#include <utility>
#include "../DraggableNode/DraggableNodeEditor.h"

DragArea::DragArea(juce::ValueTree tree, CurveManager& dynCurM) : nodes(std::move(tree)), curveManager(dynCurM) { }

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
    //std::cout<< "Edit: "<< id << " X  " << (int)x <<" y " << (int)y << std::endl;
//    draggableNodes[id]->setBounds((int)position.getX(), (int)position.getY(), 10, 10);

    DBG("Should repaint");
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

    for(const auto& child : nodes)
    {
        const auto& x = child.getProperty(DraggableNodeIdentifiers::posX);
        const auto& y = child.getProperty(DraggableNodeIdentifiers::posY);





        float radius = 5.0f;
        juce::Point<float> nodePos = juce::Point<float>((float) x, (float) y);
        std::cout << "Dist " << nodePos.getDistanceFrom(pos)  << std::endl;
        //Check the distance between the point and where I click if its less than the radius// will work if move the center
        //Currently just a box check
        if(pos.x > nodePos.x && pos.x < nodePos.x + 10 && pos.y > nodePos.y && pos.y < nodePos.y + 10)
        {
            DBG("Passed");
            selectedNodeId = child.getProperty(DraggableNodeIdentifiers::id);
            break;
        }
        else
        {
            selectedNodeId = -1;
        }
    }
}


void DragArea::mouseDrag(const juce::MouseEvent& e)
{

    DBG(typeid(e.eventComponent).name());

    if(selectedNodeId != -1)
    {
        curveManager.moveNode(selectedNodeId, e.position);
        DBG("Node Selected");
    }

}