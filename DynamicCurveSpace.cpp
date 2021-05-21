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
DynamicCurveSpace::DynamicCurveSpace(DynamicCurveManager& dynCurM) : dynamicCurveManager(dynCurM), draggableNodeEditor(getLocalBounds())
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    

    
    for(int i = 0; i < dynamicCurveManager.getNumberOfNodes();i++)
    {
        DraggableNodeEditor* node = new DraggableNodeEditor(getLocalBounds());
        draggableNodes.emplace_back(node);
        addAndMakeVisible(*draggableNodes[i]);
        
    }
    //addAndMakeVisible(&draggableNodeEditor);
    
}

DynamicCurveSpace::~DynamicCurveSpace()
{
}

void DynamicCurveSpace::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colours::black);   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    
    
    

}

void DynamicCurveSpace::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    //draggableNodeEditor.setBounds(0,0,10,10);
    
    for(int i = 0; i < dynamicCurveManager.getNumberOfNodes();i++)
    {
        draggableNodes[i]->setBounds(i * 10, 0, 10, 10);
    }

}
