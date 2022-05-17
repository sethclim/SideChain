//
//  LineEditor.hpp
//  SideChain - VST3
//
//  Created by Seth Climenhaga on 2021-11-09.
//  Copyright © 2021 Wraith Audio. All rights reserved.
//

#pragma once
#include <JuceHeader.h>

class LineEditor : public juce::Component{
    
public:
    LineEditor()= default;
    ~LineEditor() override= default;

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::blue);
        g.setColour (juce::Colours::orange);
        
        juce::Line<float> line (juce::Point<float>(0,0), juce::Point<float>(getWidth(),getHeight()));
     
        g.drawLine (line, 2.0f);
    }
};
