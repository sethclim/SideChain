/*
  ==============================================================================

    DynamicCurveEditor.cpp
    Created: 16 May 2021 11:37:51pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#include "DynamicCurveEditor.h"
#include "../../Backend/EnvelopeProcessor.h"
#include "../../Backend/CurveManager.h"
#include "../DragArea/DragArea.h"

//==============================================================================
DynamicCurveEditor::DynamicCurveEditor(CurveManager &dynCurM) : curveManager(dynCurM)
{
    nodeTree = dynCurM.nodes;
    dragArea = std::make_unique<DragArea>(nodeTree, dynCurM);

    curveManager.registerOnMoveNodeCallback([this]()
                                            { dragArea->reDraw(); });

    curveManager.initializeCurveManager();

    // curveManager.registerOnMoveNodeCallback([this](unsigned int id, juce::Point<float> position)
    //                                         { reDrawNode(id, position);

    addAndMakeVisible(*dragArea);
    dragArea->setBounds(0, 0, getWidth(), getHeight());
    // curveManager.initializeCurveManager(getWidth(), getHeight());
}

DynamicCurveEditor::~DynamicCurveEditor() = default;

void DynamicCurveEditor::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::red);
    g.drawRect(getLocalBounds(), 1);

    // juce::Path p;
    // p.startNewSubPath(0.0f, getHeight());

    // int idx = 0;
    // int numNodes = nodeTree.getNumChildren();

    // for (const auto &child : nodeTree)
    // {
    //     const auto &x = child.getProperty(DraggableNodeIdentifiers::posX);
    //     const auto &y = child.getProperty(DraggableNodeIdentifiers::posY);

    //     float xpos = (float)x * getWidth();
    //     float ypos = (float)y * getHeight();

    // p.lineTo(xpos, ypos);

    // if (idx < numNodes - 1)
    // {
    //     auto x2 = child.getSibling(1).getProperty(DraggableNodeIdentifiers::posX);
    //     auto y2 = child.getSibling(1).getProperty(DraggableNodeIdentifiers::posY);

    //     float x2pos = (float)x2 * getWidth();
    //     float y2pos = (float)y2 * getHeight();

    //     juce::Line<float> line(juce::Point<float>((float)xpos + 5, (float)ypos + 5), juce::Point<float>((float)x2pos + 5, (float)y2pos + 5));

    //     g.setColour(juce::Colours::purple);
    //     g.drawLine(line, 4.0f);
    // }

    // idx++;
    // }

    // p.lineTo(static_cast<float>(getWidth() - 20), static_cast<float>(getHeight() - 10));

    // p.lineTo(0.0, static_cast<float>(getHeight() - 10));
    // p.closeSubPath();

    // const juce::Colour c1 = juce::Colours::white.withAlpha(0.05f);
    // const juce::Colour c2 = juce::Colours::white.withAlpha(0.2f);
    // g.setGradientFill(juce::ColourGradient(c2, 0.0, 0.5, c1, 0.0, static_cast<float>(getHeight() - 2), false));
    // g.fillPath(p);
}

void DynamicCurveEditor::resized()
{
    dragArea->setBounds(0, 0, getWidth() - 50, getHeight());
}

int x_start = 0, y_start = 0;

void DynamicCurveEditor::mouseDown(const juce::MouseEvent &event)
{
    // juce::ModifierKeys modifiers = juce::ModifierKeys::getCurrentModifiers();

    // if (modifiers.isRightButtonDown())
    // {
    //     int x = event.position;
    //     int y = event.getMouseDownY();
    //     curveManager.insertNewNodeBetween(x, y);
    // }
}

void DynamicCurveEditor::reDrawNode()
{
    DBG("REDRAW NODE");
    //  std::cout<< "reDrawNode: "<< id << " X  " << (int)x <<" y " << (int)y << std::endl;
    //    if(draggableNodes[id] != nullptr)
    //        draggableNodes[id]->setBounds(x, y, 10, 10);
    repaint();
    DBG(nodeTree.toXmlString());
}

void DynamicCurveEditor::valueTreePropertyChanged(juce::ValueTree &nodeChanged, const juce::Identifier &property)
{
    //    const auto& id = nodeChanged.getProperty(DraggableNodeIdentifiers::id);
    //    const auto& x = nodeChanged.getProperty(DraggableNodeIdentifiers::posX);
    //    const auto& y = nodeChanged.getProperty(DraggableNodeIdentifiers::posY);
    //
    //    std::cout<< "Edit: X  " << (int)x <<" y " << (int)y << std::endl;
    //
    //    auto identifier = static_cast<unsigned int>((int) id);
    //    draggableNodes[identifier]->setBounds(x, y, 10, 10);
    //
    //    repaint();
}

// TODO Do I need this in the Dynamic curve editor?????
void DynamicCurveEditor::valueTreeChildAdded(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenAdded)
{
    //    int posX = childWhichHasBeenAdded.getProperty(DraggableNodeIdentifiers::posX);
    //    int posY = childWhichHasBeenAdded.getProperty(DraggableNodeIdentifiers::posY);
    //
    //    auto node = new DraggableNodeEditor( childWhichHasBeenAdded.getProperty(DraggableNodeIdentifiers::id),posX,posY,curveManager, *dragArea);
    //    draggableNodes.emplace_back(node);
    //    addAndMakeVisible(node);
    //    node->setBounds(posX, posY, 10, 10);
}

void DynamicCurveEditor::valueTreeChildRemoved(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved)
{
    jassertfalse;
}

void DynamicCurveEditor::valueTreeChildOrderChanged(juce::ValueTree &parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex)
{
    jassertfalse;
}

void DynamicCurveEditor::valueTreeParentChanged(juce::ValueTree &treeWhoseParentHasChanged)
{
    jassertfalse;
}

juce::Point<float> DynamicCurveEditor::getPointFromNode(const juce::ValueTree &vt)
{

    const auto &x = vt.getProperty(DraggableNodeIdentifiers::posX);
    const auto &y = vt.getProperty(DraggableNodeIdentifiers::posY);

    return juce::Point<float>(x, y);
}
