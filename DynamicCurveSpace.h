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
#include "DraggableNodeIdentifiers.h"

//==============================================================================
/*
*/
class DynamicCurveSpace  : public juce::Component ,public juce::ValueTree::Listener
{
public:
    DynamicCurveSpace(DynamicCurveManager&, juce::ValueTree);
    ~DynamicCurveSpace() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& event) override;
    void addNewNodeCallbackHandler(int childIndex);
    
    //=================================
    
    // All the listeners for the ValueTree class. Most of these aren't being used at the moment.
    
    void valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyHasChanged,
                                   const juce::Identifier& property) override;

    void valueTreeChildAdded (juce::ValueTree& parentTree,
                              juce::ValueTree& childWhichHasBeenAdded) override;
    
    void valueTreeChildRemoved (juce::ValueTree& parentTree,
                                juce::ValueTree& childWhichHasBeenRemoved,
                                int indexFromWhichChildWasRemoved) override;
    
    void valueTreeChildOrderChanged (juce::ValueTree& parentTreeWhoseChildrenHaveMoved,
                                     int oldIndex,
                                     int newIndex) override;
    
    void valueTreeParentChanged (juce::ValueTree& treeWhoseParentHasChanged) override;
    
    //=================================

private:
    DynamicCurveManager dynamicCurveManager;
    //DraggableNodeEditor draggableNodeEditor;
    juce::ValueTree nodeTree;
    
    std::vector<std::unique_ptr<DraggableNodeEditor>> draggableNodes;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DynamicCurveSpace)
};
