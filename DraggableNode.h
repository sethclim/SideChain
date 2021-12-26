/*
  ==============================================================================

    DraggableNode.h
    Created: 16 May 2021 9:21:42pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DraggableNode : public juce::ReferenceCountedObject
{
private:
    float posX;
    float posY;
    
public:
    DraggableNode(float x, float y);
    ~DraggableNode();
    
    float getX();
    float getY();
    
    void setX(float x);
    void setY(float y);
    
    //==============================================================================
    juce::Component* createEditor();
};
