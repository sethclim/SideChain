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
}
DynamicCurveManager::~DynamicCurveManager(){
 
}


juce::Component* DynamicCurveManager::createEditor()
{
    return new DynamicCurveSpace(*this);
}

int DynamicCurveManager::getNumberOfNodes(){
    return numberOfNodes;
}
