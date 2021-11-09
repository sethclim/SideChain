/*
  ==============================================================================

    DynamicCurveSpace.cpp
    Created: 16 May 2021 11:37:51pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DynamicCurveSpace.h"
//==============================================================================
DynamicCurveSpace::DynamicCurveSpace(DynamicCurveManager& dynCurM, juce::ValueTree nodes) :dynamicCurveManager(dynCurM),nodeTree(nodes)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    for(int i = 0; i < dynamicCurveManager.getNumberOfNodes();i++)
    {
        DraggableNodeEditor* node = new DraggableNodeEditor(getLocalBounds());
        draggableNodes.emplace_back(node);
        addAndMakeVisible(*draggableNodes[i]);
    }
    
    nodeTree.addListener(this);
    
}

DynamicCurveSpace::~DynamicCurveSpace()
{
}

void DynamicCurveSpace::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);

}

void DynamicCurveSpace::resized()
{
    
    for(int i = 0; i < dynamicCurveManager.getNumberOfNodes();i++)
    {
        draggableNodes[i]->setBounds(i * 10, 0, 10, 10);
    }

}

void DynamicCurveSpace::mouseDown (const juce::MouseEvent& event)
{
    juce::ModifierKeys modifiers = juce::ModifierKeys::getCurrentModifiers();
    
    if (modifiers.isRightButtonDown())
    {
        float x = event.getMouseDownX();
        float y = event.getMouseDownY();
        
        dynamicCurveManager.addNewNode(x,y);
    }
        
}

void DynamicCurveSpace::addNewNodeCallbackHandler(int childIndex)
{
    int numNodes = nodeTree.getProperty(DraggableNodeIdentifiers::numberOfNodes);
    
    for(int i = 0; i < numNodes;i++)
    {
        DraggableNodeEditor* node = new DraggableNodeEditor(getLocalBounds());
        addAndMakeVisible(node);
        node->setBounds(i * 10, 0, 10, 10);
        
    }
    
}

void DynamicCurveSpace::valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    //DBG(treeWhosePropertyHasChanged.toXmlString());
   //addNewNodeCallbackHandler(nodeTree.indexOf(treeWhosePropertyHasChanged));
}

void DynamicCurveSpace::valueTreeChildAdded (juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded)
{
    float posX = childWhichHasBeenAdded.getProperty(DraggableNodeIdentifiers::posX);
    float posY = childWhichHasBeenAdded.getProperty(DraggableNodeIdentifiers::posY);
    
    DraggableNodeEditor* node = new DraggableNodeEditor(getLocalBounds());
    draggableNodes.emplace_back(node);
    addAndMakeVisible(node);
    node->setBounds(posX, posY, 10, 10);
}

void DynamicCurveSpace::valueTreeChildRemoved (juce::ValueTree& parentTree,juce::ValueTree& childWhichHasBeenRemoved,int indexFromWhichChildWasRemoved)
{
    jassertfalse;
}

void DynamicCurveSpace::valueTreeChildOrderChanged (juce::ValueTree& parentTreeWhoseChildrenHaveMoved,int oldIndex,int newIndex)
{
    jassertfalse;
}

void DynamicCurveSpace::valueTreeParentChanged (juce::ValueTree& treeWhoseParentHasChanged)
{
    jassertfalse;
}

