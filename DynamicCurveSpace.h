/*
  ==============================================================================

    DynamicCurveSpace.h
    Created: 16 May 2021 11:37:51pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DraggableNodeEditor.h"

//==============================================================================
/*
*/
class DynamicCurveSpace  : public juce::Component
{
public:
    DynamicCurveSpace();
    ~DynamicCurveSpace() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    DraggableNodeEditor draggableNodeEditor;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DynamicCurveSpace)
};
