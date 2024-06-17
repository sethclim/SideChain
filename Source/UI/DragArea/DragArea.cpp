//
// Created by Seth Climenhaga on 2022-05-21.
//
//
#include "DragArea.h"
#include <utility>

DragArea::DragArea(juce::AudioProcessorValueTreeState &apvts, CurveManager &curveManager) : apvts(apvts), m_CurveManager(curveManager)
{
    setWantsKeyboardFocus(true);
}

DragArea::~DragArea() = default;

void DragArea::StartListening()
{
    apvts.state.addListener(this);
}

void DragArea::paint(juce::Graphics &g)
{
    juce::Path p;
    p.startNewSubPath(0.0f, getHeight() - 10);

    int idx = 0;
    int numNodes = apvts.state.getChildWithName(DraggableNodeIdentifiers::myRootDraggableTreeType).getNumChildren();

    float adj_width = getWidth() - 10;
    float adj_height = getHeight() - 10;

    for (const auto &child : apvts.state.getChildWithName(DraggableNodeIdentifiers::myRootDraggableTreeType))
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

            g.setColour(juce::Colour::fromRGBA(255, 255, 255, 255));
            g.drawLine(line, 4.0f);
        }

        // std::cout << "xpos " << xpos << " ypos " << ypos << std::endl;
        if ((int)child.getProperty(DraggableNodeIdentifiers::id) == selectedNodeId)
        {
            g.setColour(juce::Colours::black);
            g.fillEllipse(xpos, ypos, m_CurveManager.controlSize, m_CurveManager.controlSize);
        }
        else
        {
            g.setColour(juce::Colours::white);
            g.fillEllipse(xpos, ypos, m_CurveManager.controlSize, m_CurveManager.controlSize);
        }
        g.setColour(juce::Colours::white);
        g.drawEllipse(xpos, ypos, m_CurveManager.controlSize, m_CurveManager.controlSize, 2);

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
    for (const auto &child : apvts.state.getChildWithName(DraggableNodeIdentifiers::myRootDraggableTreeType))
    {
        float x = (float)child.getProperty(DraggableNodeIdentifiers::posX);
        float y = (float)child.getProperty(DraggableNodeIdentifiers::posY);

        float adj_width = getWidth() - 10;
        float adj_height = getHeight() - 10;
        // Check the distance between the point and where I click if its less than the radius// will work if move the center
        // Currently just a box check
        if (pos.x > (x * adj_width) && pos.x < (x * adj_width) + 10 && pos.y > (y * adj_height) && pos.y < (y * adj_height) + 10)
        {
            selectedNodeId = child.getProperty(DraggableNodeIdentifiers::id);
            DBG("SELECTED");
            repaint();
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
        m_CurveManager.insertNewNodeBetween(scaleToCoord(pos));
    }
}

void DragArea::mouseDrag(const juce::MouseEvent &e)
{
    if (selectedNodeId != -1)
    {
        m_CurveManager.moveNode(selectedNodeId, scaleToCoord(e.position));
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
    float scaled_x = position.x / ((float)getWidth() - 10);
    float scaled_y = position.y / ((float)getHeight() - 10);

    return juce::Point<float>(scaled_x, scaled_y);
}

void DragArea::SetAddMode(bool mode)
{
    addMode = mode;
}

bool DragArea::GetAddMode()
{
    return addMode;
}

void DragArea::valueTreeRedirected(ValueTree &treeWhichHasBeenChanged)
{
    repaint();
}
