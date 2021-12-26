/*
  ==============================================================================

    DynamicCurveEditor.h
    Created: 16 May 2021 11:37:16pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "DraggableNode.h"
#include "DraggableNodeIdentifiers.h"

class DynamicCurve //: public juce::ActionBroadcaster
{
private:
    int numberOfNodes;
    
     std::vector<DraggableNode> nodes;
    
public:
    DynamicCurve();
    ~DynamicCurve();
    //==============================================================================
//    void addActionListener (juce::ActionListener *listener);
//    void removeActionListener (juce::ActionListener *listener)
//    void sendActionMessage (const String &message) const;
//
    //==============================================================================
    juce::Component* createEditor();
    
    
    //==============================================================================
    int getNumberOfNodes();
    void setNumberOfNodes();
    void addNewNode(float x, float y);
    
    
    
    
    juce::ValueTree draggableNodes;
};
