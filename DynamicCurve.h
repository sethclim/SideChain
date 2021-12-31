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
#include "DataPoint.h"
#include "Transport.h"


class DynamicCurve //: public juce::ActionBroadcaster
{
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
    void calculateDataPointsFromTree(float width, float height);
    std::vector<DataPoint> getSegments();
    void ProcessAudio(float* channelData, int numSamples,Transport& transport);
    
    juce::ValueTree draggableNodes;
    std::vector<DataPoint> segments;

    
private:
    int numberOfNodes;
    std::vector<DraggableNode> nodes;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DynamicCurve)
};
