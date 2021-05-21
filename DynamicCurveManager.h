/*
  ==============================================================================

    DynamicCurveSpace.h
    Created: 16 May 2021 11:37:16pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class DynamicCurveManager
{
private:
    int numberOfNodes;
    
public:
    DynamicCurveManager();
    ~DynamicCurveManager();
    
    //==============================================================================
    juce::Component* createEditor();
    
    int getNumberOfNodes();
    
};
