//
//  LineEditor.cpp
//  SideChain - VST3
//
//  Created by Seth Climenhaga on 2021-11-09.
//  Copyright © 2021 Wraith Audio. All rights reserved.
//

#include "LineEditor.hpp"

//==============================================================================
LineEditor::LineEditor()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

LineEditor::~LineEditor()
{
}

void LineEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::blue);
    g.setColour (juce::Colours::orange);
    
   // printf("Paint Point start:XY %0.2f + %0.2f end:XY %0.2f + %0.2f \n", start.x, start.y, end.x, end.y);
    
    juce::Line<float> line (juce::Point<float>(0,0), juce::Point<float>(getWidth(),getHeight()));
 
    g.drawLine (line, 2.0f);

}

void LineEditor::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

//void LineEditor::setFocusContainerType (new FocusContainerType(none)){
//
//}

