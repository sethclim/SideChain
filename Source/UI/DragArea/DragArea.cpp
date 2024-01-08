//
// Created by Seth Climenhaga on 2022-05-21.
//
//
#include "DragArea.h"
#include <utility>

DragArea::DragArea(const juce::ValueTree &tree, CurveManager &dynCurM) : nodes(tree), curveManager(dynCurM)
{
    setWantsKeyboardFocus(true);
    std::cout << "CONTROL SIZE" << curveManager.controlSize << std::endl;
    DBG(nodes.toXmlString());
}

DragArea::~DragArea() = default;

void DragArea::paint(juce::Graphics &g)
{
    // g.fillAll(juce::Colours::purple);

    for (const auto &child : nodes)
    {
        const auto &x = child.getProperty(DraggableNodeIdentifiers::posX);
        const auto &y = child.getProperty(DraggableNodeIdentifiers::posY);

        float xpos = (float)x * getWidth();
        float ypos = (float)y * getHeight();

        std::cout << "xpos " << xpos << " ypos " << ypos << std::endl;
        g.setColour(juce::Colours::orange);
        g.fillEllipse(xpos, ypos, curveManager.controlSize, curveManager.controlSize);
        g.drawEllipse(xpos, ypos, curveManager.controlSize, curveManager.controlSize, 1);
    }
}

void DragArea::reDraw()
{
    repaint();
}

void DragArea::resized()
{
    std::cout << "width " << getWidth() << " height " << getHeight() << std::endl;
}

void DragArea::mouseDown(const juce::MouseEvent &event)
{
    juce::Point<float> pos = event.position;

    // Check for a selected node and set it
    for (const auto &child : nodes)
    {
        float x = (float)child.getProperty(DraggableNodeIdentifiers::posX);
        float y = (float)child.getProperty(DraggableNodeIdentifiers::posY);

        juce::Point<float> nodePos = juce::Point<float>((float)x, (float)y);
        // Check the distance between the point and where I click if its less than the radius// will work if move the center
        // Currently just a box check
        if (pos.x > (x * getWidth()) && pos.x < (x * getWidth()) + 10 && pos.y > (y * getHeight()) && pos.y < (y * getHeight()) + 10)
        {
            selectedNodeId = child.getProperty(DraggableNodeIdentifiers::id);
            break;
        }
        else
        {
            selectedNodeId = -1;
        }
    }

    juce::ModifierKeys modifiers = juce::ModifierKeys::getCurrentModifiers();

    if (selectedNodeId == -1 && addMode && modifiers.isRightButtonDown())
    {
        curveManager.insertNewNodeBetween(scaleToCoord(pos));
    }
}

void DragArea::mouseDrag(const juce::MouseEvent &e)
{
    if (selectedNodeId != -1)
    {
        curveManager.moveNode(selectedNodeId, scaleToCoord(e.position));
    }
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

juce::Point<float> DragArea::scaleToCoord(juce::Point<float> position)
{
    float scaled_x = position.x / (float)getWidth();
    float scaled_y = position.y / (float)getHeight();

    return juce::Point<float>(scaled_x, scaled_y);
}