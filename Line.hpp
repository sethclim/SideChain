//
//  Line.hpp
//  SideChain - VST3
//
//  Created by Seth Climenhaga on 2021-11-09.
//  Copyright © 2021 Wraith Audio. All rights reserved.
//

#pragma once
#include <JuceHeader.h>
#include "Point.h"
#include "LineEditor.hpp"


class Line{
    juce::Point<float>  start, end;
    
public:
    
    Line(juce::Point<float>  start, juce::Point<float>  end);
    
//    juce::Component* createEditor();
};
