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
    
    containerBounds = new juce::ComponentBoundsConstrainer();
    containerBounds->setMinimumOnscreenAmounts(15, 15, 15, 15);
    //containerBounds->setSizeLimits(10, 10, 50, 50);
    
    this->id = id;
}

DraggableNodeEditor::~DraggableNodeEditor()
{
}

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

 
void DraggableNodeEditor::mouseDown (const juce::MouseEvent& e)
{
    myDragger.startDraggingComponent (this, e);
}

void DraggableNodeEditor::mouseDrag (const juce::MouseEvent& e)
{
    myDragger.dragComponent (this, e, containerBounds);
    auto node = nodeTree.getChildWithProperty(DraggableNodeIdentifiers::id, id);
    node.setProperty(DraggableNodeIdentifiers::posX, this->getX() , nullptr);
    node.setProperty(DraggableNodeIdentifiers::posY, this->getY() , nullptr);
}
