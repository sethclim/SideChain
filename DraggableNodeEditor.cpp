/*
  ==============================================================================

    DraggableNode.cpp
    Created: 16 May 2021 9:20:05pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DraggableNodeEditor.h"

//==============================================================================
DraggableNodeEditor::DraggableNodeEditor(juce::Rectangle<int> bounds, juce::ValueTree nodes, int id) : nodeTree(nodes)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    this->id = id;
    this->parentBounds = bounds;
}

DraggableNodeEditor::~DraggableNodeEditor(){}

void DraggableNodeEditor::paint (juce::Graphics& g)
{
    //g.fillAll (juce::Colours::blue);   // clear the background

    g.setColour (juce::Colours::orange);
    g.fillEllipse(0, 0, 10, 10);
    g.drawEllipse(0, 0, 10, 10, 1);
}

void DraggableNodeEditor::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}


void DraggableNodeEditor::mouseDrag (const juce::MouseEvent& e)
{
   
    auto node = nodeTree.getChildWithProperty(DraggableNodeIdentifiers::id, id);
    
    int minWidth = 0;
    int maxWidth = parentBounds.getWidth() - 10;
    int minHeight = 0;
    int maxHeight = parentBounds.getHeight() - 10;
    
    if((int)node.getProperty(DraggableNodeIdentifiers::id) == 0)
    {
        maxWidth = 10;
    }
    else if((int)node.getProperty(DraggableNodeIdentifiers::id) == 1)
    {
        minWidth = parentBounds.getWidth() - 10 - 10;
    }
    else
    {
        auto prevSib = node.getSibling(-1);
        auto nextSib = node.getSibling(1);
        
        if(prevSib.isValid()){
            minWidth = prevSib.getProperty(DraggableNodeIdentifiers::posX);
        }
        
        if(nextSib.isValid()){
            maxWidth = nextSib.getProperty(DraggableNodeIdentifiers::posX);
        }
        
        
    }
    
    auto newX = this->getX();
    auto newY =  this->getY();
    
    newX += e.x - 5;
    newY += e.y - 5;
    
    if(minWidth <= newX && newX <= maxWidth)
    {
        node.setProperty(DraggableNodeIdentifiers::posX,newX, nullptr);
    }
    
    if(minHeight <= newY && newY <= maxHeight)
    {
        node.setProperty(DraggableNodeIdentifiers::posY,newY, nullptr);
    }
}

void DraggableNodeEditor::setParentBounds(juce::Rectangle<int> bounds)
{
    parentBounds = bounds;
}
