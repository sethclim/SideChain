/*
  ==============================================================================

    DynamicCurveEditor.h
    Created: 16 May 2021 11:37:51pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// class CurveManager;
// class DraggableNodeEditor;
// class DragArea;

#include "../DragArea/DragArea.h"

//==============================================================================
class DynamicCurveEditor : public juce::Component
{
public:
  explicit DynamicCurveEditor(CurveManager &, juce::AudioProcessorValueTreeState &);
  ~DynamicCurveEditor() override;

  void paint(juce::Graphics &) override;
  void resized() override;
  void mouseDown(const juce::MouseEvent &event) override;
  void reDrawNode();

  bool GetDragAreaMode();
  void SetDragAreaMode(bool mode);

private:
  CurveManager &m_CurveManager;
  // juce::AudioProcessorValueTreeState nodeTree;
  static juce::Point<float> getPointFromNode(const juce::ValueTree &);
  DragArea dragArea;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DynamicCurveEditor)
};
