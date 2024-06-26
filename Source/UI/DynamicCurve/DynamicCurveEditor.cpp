/*
  ==============================================================================

    DynamicCurveEditor.cpp
    Created: 16 May 2021 11:37:51pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#include "DynamicCurveEditor.h"
#include "../../Backend/CurveManager.h"
#include "../DragArea/DragArea.h"

//==============================================================================
DynamicCurveEditor::DynamicCurveEditor(CurveManager &curveManager, juce::AudioProcessorValueTreeState &apvts) : m_CurveManager(curveManager),
                                                                                                                dragArea(apvts, curveManager)
{
    m_CurveManager.registerOnMoveNodeCallback([this]()
                                              { dragArea.reDraw(); });
    dragArea.StartListening();

    addAndMakeVisible(dragArea);
    dragArea.setBounds(0, 0, getWidth(), getHeight());
}

DynamicCurveEditor::~DynamicCurveEditor() = default;

void DynamicCurveEditor::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::black);
    // g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds(), 1);
}

void DynamicCurveEditor::resized()
{
    dragArea.setBounds(0, 0, getWidth(), getHeight());
}

int x_start = 0, y_start = 0;

void DynamicCurveEditor::mouseDown(const juce::MouseEvent &event)
{
}

void DynamicCurveEditor::reDrawNode()
{
    repaint();
}

juce::Point<float> DynamicCurveEditor::getPointFromNode(const juce::ValueTree &vt)
{

    const auto &x = vt.getProperty(DraggableNodeIdentifiers::posX);
    const auto &y = vt.getProperty(DraggableNodeIdentifiers::posY);

    return juce::Point<float>(x, y);
}

bool DynamicCurveEditor::GetDragAreaMode() { return dragArea.GetAddMode(); }
void DynamicCurveEditor::SetDragAreaMode(bool mode) { dragArea.SetAddMode(mode); }