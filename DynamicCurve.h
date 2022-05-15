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
    DynamicCurve(Transport& t);
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
    //float getNextSample() noexcept;
    std::vector<DataPoint> getSegments();
    
    void ApplySideChainToBuffer(juce::AudioBuffer<float>& buffer, int startSample, int numSamples);
    
    Transport& transport;
    
    juce::ValueTree draggableNodes;
    std::vector<DataPoint> segments;
    std::atomic<double> currentVol {0};
    std::atomic<double> relPosition {0};
    
private:
    int numberOfNodes;
    std::vector<DraggableNode> nodes;
    float getNextSample() noexcept;
    float relativePosition  = 0.0;
    int idx = 0;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DynamicCurve)
};
