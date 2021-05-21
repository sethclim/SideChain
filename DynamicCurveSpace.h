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
#include "DynamicCurveManager.h"

//==============================================================================
/*
*/
class DynamicCurveSpace  : public juce::Component
{
public:
    DynamicCurveSpace(DynamicCurveManager&);
    ~DynamicCurveSpace() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    DynamicCurveManager& dynamicCurveManager;
    DraggableNodeEditor draggableNodeEditor;
    
    std::vector<std::unique_ptr<DraggableNodeEditor>> draggableNodes;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DynamicCurveSpace)
};
