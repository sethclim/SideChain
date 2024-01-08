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

    juce::Path p;
    p.startNewSubPath(0.0f, getHeight());

    int idx = 0;
    int numNodes = nodes.getNumChildren();

    float adj_width = getWidth() - 10;
    float adj_height = getHeight() - 10;

    for (const auto &child : nodes)
    {
        const auto &x = child.getProperty(DraggableNodeIdentifiers::posX);
        const auto &y = child.getProperty(DraggableNodeIdentifiers::posY);

        float xpos = (float)x * adj_width;
        float ypos = (float)y * adj_height;
        p.lineTo(xpos, ypos);
        if (idx < numNodes - 1)
        {
            auto x2 = child.getSibling(1).getProperty(DraggableNodeIdentifiers::posX);
            auto y2 = child.getSibling(1).getProperty(DraggableNodeIdentifiers::posY);

            float x2pos = (float)x2 * adj_width;
            float y2pos = (float)y2 * adj_height;

            juce::Line<float> line(juce::Point<float>((float)xpos + 5, (float)ypos + 5), juce::Point<float>((float)x2pos + 5, (float)y2pos + 5));

            g.setColour(juce::Colours::purple);
            g.drawLine(line, 4.0f);
        }

        std::cout << "xpos " << xpos << " ypos " << ypos << std::endl;
        g.setColour(juce::Colours::orange);
        g.fillEllipse(xpos, ypos, curveManager.controlSize, curveManager.controlSize);
        g.drawEllipse(xpos, ypos, curveManager.controlSize, curveManager.controlSize, 1);

        idx++;
    }

    p.lineTo(static_cast<float>(adj_width), static_cast<float>(adj_height));

    p.lineTo(0.0, static_cast<float>(adj_height));
    p.closeSubPath();

    const juce::Colour c1 = juce::Colours::white.withAlpha(0.05f);
    const juce::Colour c2 = juce::Colours::white.withAlpha(0.2f);
    g.setGradientFill(juce::ColourGradient(c2, 0.0, 0.5, c1, 0.0, static_cast<float>(adj_height - 2), false));
    g.fillPath(p);
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
        float adj_width = getWidth() - 10;
        float adj_height = getWidth() - 10;
        // Check the distance between the point and where I click if its less than the radius// will work if move the center
        // Currently just a box check
        if (pos.x > (x * adj_width) && pos.x < (x * adj_width) + 10 && pos.y > (y * adj_height) && pos.y < (y * adj_height) + 10)
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