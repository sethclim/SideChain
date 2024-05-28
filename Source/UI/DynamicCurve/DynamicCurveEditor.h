/*
  ==============================================================================

    DynamicCurveEditor.h
    Created: 16 May 2021 11:37:51pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CurveManager;
class DraggableNodeEditor;
class DragArea;

//==============================================================================
class DynamicCurveEditor  : public juce::Component, public juce::ValueTree::Listener
{
public:
    explicit DynamicCurveEditor(CurveManager&);
    ~DynamicCurveEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& event) override;
    
    //=============================================================================================
    
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
    
    //=============================================================================================
    void reDrawNode();

    bool GetDragAreaMode();
    void SetDragAreaMode(bool mode);

private:
    CurveManager& curveManager;
    juce::ValueTree nodeTree;
    static juce::Point<float> getPointFromNode(const juce::ValueTree&);
    std::unique_ptr<DragArea> dragArea;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DynamicCurveEditor)
};
