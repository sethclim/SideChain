//
// Created by Seth Climenhaga on 2022-05-21.
//
//
#include "DragArea.h"
#include <utility>

DragArea::DragArea(const juce::ValueTree &tree, CurveManager &dynCurM) : nodes(tree), curveManager(dynCurM)
{
    setWantsKeyboardFocus(true);
}

DragArea::~DragArea() = default;

void DragArea::paint(juce::Graphics &g)
{
    for (const auto &child : nodes)
    {
        const auto &x = child.getProperty(DraggableNodeIdentifiers::posX);
        const auto &y = child.getProperty(DraggableNodeIdentifiers::posY);

        g.setColour(juce::Colours::orange);
        g.fillEllipse(x, y, 10, 10);
        g.drawEllipse(x, y, 10, 10, 1);
    }
}

void DragArea::reDrawNode(unsigned int id, juce::Point<float> position)
{
    repaint();
}

void DragArea::resized() {}

void DragArea::mouseDown(const juce::MouseEvent &event)
{
    juce::Point<float> pos = event.position;

    // Check for a selected node and set it
    for (const auto &child : nodes)
    {
        const auto &x = child.getProperty(DraggableNodeIdentifiers::posX);
        const auto &y = child.getProperty(DraggableNodeIdentifiers::posY);

        juce::Point<float> nodePos = juce::Point<float>((float)x, (float)y);
        // Check the distance between the point and where I click if its less than the radius// will work if move the center
        // Currently just a box check
        if (pos.x > nodePos.x && pos.x < nodePos.x + 10 && pos.y > nodePos.y && pos.y < nodePos.y + 10)
        {
            selectedNodeId = child.getProperty(DraggableNodeIdentifiers::id);
            break;
        }
        else
        {
            selectedNodeId = -1;
        }
    }

    if (selectedNodeId == -1 && addMode)
    {
        curveManager.addNewNode(static_cast<int>(pos.x), static_cast<int>(pos.y));
    }
}

void DragArea::mouseDrag(const juce::MouseEvent &e)
{
    if (selectedNodeId != -1)
        curveManager.moveNode(selectedNodeId, e.position);
}

bool DragArea::keyPressed(const juce::KeyPress &key)
{
    if (key == juce::KeyPress::escapeKey)
    {
        addMode = !addMode;
        std::cout << addMode << std::endl;
        return true;
    }
    return false;
}