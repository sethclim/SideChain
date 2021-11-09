/*
  ==============================================================================

    DynamicCurveSpace.cpp
    Created: 16 May 2021 11:37:16pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#include "DynamicCurveManager.h"
#include "DynamicCurveSpace.h"

DynamicCurveManager::DynamicCurveManager(){
    numberOfNodes = 8;
    
    juce::ValueTree myNodes (DraggableNodeIdentifiers::myRootDraggableTreeType);
    draggableNodes = myNodes;
    
}
DynamicCurveManager::~DynamicCurveManager(){
 
}

juce::Component* DynamicCurveManager::createEditor()
{
    return new DynamicCurveSpace(*this, draggableNodes);
}

int DynamicCurveManager::getNumberOfNodes(){
    return numberOfNodes;
}
void DynamicCurveManager::addNewNode(float x, float y){
    
    
    DraggableNode* dNode = new DraggableNode(x,y);

    //nodes.emplace_back(dNode);
    numberOfNodes++;
    
    if (draggableNodes.isValid())
    {
        juce::ValueTree myNode (DraggableNodeIdentifiers::myNodeType);
        
        
        myNode.setProperty(DraggableNodeIdentifiers::posX, dNode->getX(), nullptr);
        myNode.setProperty(DraggableNodeIdentifiers::posY, dNode->getY(), nullptr);
        
        draggableNodes.addChild(myNode, -1, nullptr);
        
    }
}
