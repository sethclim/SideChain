/*
  ==============================================================================

    DraggableNode.h
    Created: 16 May 2021 9:20:05pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../DraggableNodeIdentifiers.h"

//==============================================================================
class DraggableNodeEditor  : public juce::Component
{
public:
    DraggableNodeEditor(juce::Rectangle<int> bounds, juce::ValueTree nodes, int id) : nodeTree(nodes)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        this->id = id;
        this->parentBounds = bounds;
    }

   ~DraggableNodeEditor() override{}

    void paint (juce::Graphics& g)
    {
        //g.fillAll (juce::Colours::blue);   // clear the background

        g.setColour (juce::Colours::orange);
        g.fillEllipse(0, 0, 10, 10);
        g.drawEllipse(0, 0, 10, 10, 1);
    }


    void mouseDrag (const juce::MouseEvent& e) override
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

        // Replace this with call to curve manager move
    }

    void setParentBounds(juce::Rectangle<int> bounds)
    {
        parentBounds = bounds;
    }

private:
    int id;
    juce::ValueTree nodeTree;
    juce::Rectangle<int> parentBounds;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DraggableNodeEditor)
};
