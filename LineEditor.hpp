//
//  LineEditor.hpp
//  SideChain - VST3
//
//  Created by Seth Climenhaga on 2021-11-09.
//  Copyright © 2021 Wraith Audio. All rights reserved.
//

#pragma once
#include <JuceHeader.h>
#include "Point.h"
#include "Line.hpp"

class LineEditor  : public juce::Component{
    
public:
    LineEditor();
    ~LineEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    juce::Component* createEditor();
    
//    void setFocusContainerType(FocusContainerType  containerType);
    
   //juce::Point<float>  start, end;

};
