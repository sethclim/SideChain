/*
  ==============================================================================

    DynamicCurveEditor.cpp
    Created: 16 May 2021 11:37:16pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#include "DynamicCurve.h"
#include "DynamicCurveEditor.h"

DynamicCurve::DynamicCurve(){
    numberOfNodes = 0;
    
    juce::ValueTree myNodes (DraggableNodeIdentifiers::myRootDraggableTreeType);
    draggableNodes = myNodes;
    
    addNewNode(10,10);
    addNewNode(100,100);
    
}
DynamicCurve::~DynamicCurve(){
 
}

juce::Component* DynamicCurve::createEditor()
{
    return new DynamicCurveEditor(*this, draggableNodes);
}

int DynamicCurve::getNumberOfNodes(){
    return numberOfNodes;
}
void DynamicCurve::addNewNode(float x, float y){
    
    DraggableNode* dNode = new DraggableNode(x,y);

    //nodes.emplace_back(dNode);
    
    if (draggableNodes.isValid())
    {
        juce::ValueTree myNode (DraggableNodeIdentifiers::myNodeType);
        
        
        myNode.setProperty(DraggableNodeIdentifiers::posX, dNode->getX(), nullptr);
        myNode.setProperty(DraggableNodeIdentifiers::posY, dNode->getY(), nullptr);
        myNode.setProperty(DraggableNodeIdentifiers::id, numberOfNodes, nullptr);
        
        //DBG(draggableNodes.toXmlString());
        
        // get the right index to land the node between two other nodes
        
        bool slotFound = false;
        
        // X ----- here ----- X //
        for(const auto& child : draggableNodes)
        {
            auto child_X = child.getProperty(DraggableNodeIdentifiers::posX);
            auto childNext_X = child.getSibling(1).getProperty(DraggableNodeIdentifiers::posX);
            
            if((float)child_X < x && x < (float)childNext_X)
            {
               auto childIdx =  draggableNodes.indexOf(child);
                
                draggableNodes.addChild(myNode, childIdx + 1, nullptr);
                numberOfNodes++;
                slotFound = true;
                break;
            }
        }
        
        if(!slotFound)
        {
            draggableNodes.addChild(myNode, -1, nullptr);
            numberOfNodes++;
        }
    }
}
