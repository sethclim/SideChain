//
// Created by Seth Climenhaga on 2022-05-21.
//

#include <JuceHeader.h>
#include "DraggableNodeEditor.h"
#include "../../Backend/CurveManager.h"
#include "../DragArea/DragArea.h"

DraggableNodeEditor::DraggableNodeEditor(int identifier,int x, int y, CurveManager& cM, DragArea& dA ) : curveManager(cM), dragArea(dA)
{
    this->id = identifier;
    this->startX = x;
    this->startY = y;
}

DraggableNodeEditor::~DraggableNodeEditor()= default;

void DraggableNodeEditor::paint(juce::Graphics& g)
{
    g.setColour (juce::Colours::orange);
    g.fillEllipse(0, 0, 10, 10);
    g.drawEllipse(0, 0, 10, 10, 1);
}

void DraggableNodeEditor::mouseDown(const juce::MouseEvent& event)
{
    juce::ModifierKeys modifiers = juce::ModifierKeys::getCurrentModifiers();

    if (modifiers.isLeftButtonDown())
    {
        startX = event.getMouseDownX();
        startY = event.getMouseDownY();
    }
}

void DraggableNodeEditor::mouseDrag (const juce::MouseEvent& e)
{
    e.getEventRelativeTo(&dragArea);

    float x = static_cast<float>(e.getPosition().getX());
    float y = static_cast<float>(e.getPosition().getY());

    std::cout << "mouseDrag: X " << x << " Y " << y << std::endl;

    //curveManager.moveNode(id, startX, startY, x, y);
}
