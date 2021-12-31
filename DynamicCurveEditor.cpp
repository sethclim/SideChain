/*
  ==============================================================================

    DynamicCurveEditor.cpp
    Created: 16 May 2021 11:37:51pm
    Author:  Seth Climenhaga
 
  ==============================================================================
*/

#include <JuceHeader.h>
#include "DynamicCurveEditor.h"
//==============================================================================
DynamicCurveEditor::DynamicCurveEditor(DynamicCurve& dynCurM, juce::ValueTree nodes) :dynamicCurve(dynCurM),nodeTree(nodes)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    int i = 0;
    for(const auto& child : nodeTree)
    {
        if(i < nodeTree.getNumChildren() - 1){
            LineEditor* line = new LineEditor();
            
            lines.emplace_back(line);
            addAndMakeVisible(*lines[i]);
        }
        
        DraggableNodeEditor* node = new DraggableNodeEditor(getLocalBounds(), nodeTree, child.getProperty(DraggableNodeIdentifiers::id));
        node->setParentBounds(this->getBounds());
        draggableNodes.emplace_back(node);
        addAndMakeVisible(*draggableNodes[i]);
        
        i++;
    }
    nodeTree.addListener(this);
}

DynamicCurveEditor::~DynamicCurveEditor(){}

void DynamicCurveEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);
    
    
    int idx = 0;
    int jdx = 0;
    int numNodes = nodeTree.getNumChildren();
    
    for(const auto& child : nodeTree)
    {
        auto x = child.getProperty(DraggableNodeIdentifiers::posX);
        auto y = child.getProperty(DraggableNodeIdentifiers::posY);
        
        
        if(jdx < numNodes - 1){
            auto x2 = child.getSibling(1).getProperty(DraggableNodeIdentifiers::posX);
            auto y2 = child.getSibling(1).getProperty(DraggableNodeIdentifiers::posY);
            
            juce::Line<float> line (juce::Point<float>((int)x + 5 ,(int)y + 5 ), juce::Point<float>((int)x2 + 5,(int)y2 + 5));
         
            g.setColour (juce::Colours::orange);
            g.drawLine (line, 4.0f);
        }
        
        idx++;
        jdx++;
    }
}

void DynamicCurveEditor::resized()
{
    auto zero = nodeTree.getChildWithProperty(DraggableNodeIdentifiers::id, 0);
    auto one = nodeTree.getChildWithProperty(DraggableNodeIdentifiers::id, 1);
    
    zero.setProperty(DraggableNodeIdentifiers::posY, this->getHeight() - 10, nullptr);
    
    one.setProperty(DraggableNodeIdentifiers::posX, this->getWidth() - 10, nullptr);
    one.setProperty(DraggableNodeIdentifiers::posY, this->getHeight() - 10, nullptr);
    
    int idx = 0;
    int jdx = 0;
    
    for(const auto& child : nodeTree)
    {
        auto x = child.getProperty(DraggableNodeIdentifiers::posX);
        auto y = child.getProperty(DraggableNodeIdentifiers::posY);
        
        draggableNodes[idx]->setBounds(x ,y, 10, 10);
        draggableNodes[idx]->setParentBounds(this->getBounds());

        idx++;
        jdx++;
    }
}

void DynamicCurveEditor::mouseDown (const juce::MouseEvent& event)
{
    juce::ModifierKeys modifiers = juce::ModifierKeys::getCurrentModifiers();
    
    if (modifiers.isRightButtonDown())
    {
        float x = event.getMouseDownX();
        float y = event.getMouseDownY();
    
        dynamicCurve.addNewNode(x,y);
    }
        
}

void DynamicCurveEditor::valueTreePropertyChanged (juce::ValueTree& nodeChanged, const juce::Identifier& property)
{
    auto id = nodeChanged.getProperty(DraggableNodeIdentifiers::id);
    auto x = nodeChanged.getProperty(DraggableNodeIdentifiers::posX);
    auto y = nodeChanged.getProperty(DraggableNodeIdentifiers::posY);
    
    int minWidth = 0;
    int maxWidth = this->getWidth();
    
    int minHeight = 0;
    int maxHeight = this->getHeight();
    
    auto prevSib = nodeChanged.getSibling(-1);
    auto nextSib = nodeChanged.getSibling(1);
    
    if(prevSib.isValid()){
        minWidth = prevSib.getProperty(DraggableNodeIdentifiers::posX);
    }
    
    if(nextSib.isValid()){
        maxWidth = nextSib.getProperty(DraggableNodeIdentifiers::posX);
    }
    
    std::cout<< "X " << (int)x <<" Y "<< (int)y << " MinWidth " << (int)minWidth << " MaxWidth " << (int)maxWidth << std::endl;
    
    if(minWidth <= (int) x && (int) x <= maxWidth  && minHeight <= (int) y && (int) y <= maxHeight )
    {
        std::cout<< "Passed the Check!" << std::endl;
        draggableNodes[(int)id]->setBounds(x, y, 10, 10);
       
    }
    
    repaint();

    dynamicCurve.calculateDataPointsFromTree((float) this->getWidth(), (float) this->getHeight());
}

void DynamicCurveEditor::valueTreeChildAdded (juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded)
{
    float posX = childWhichHasBeenAdded.getProperty(DraggableNodeIdentifiers::posX);
    float posY = childWhichHasBeenAdded.getProperty(DraggableNodeIdentifiers::posY);
    
    DraggableNodeEditor* node = new DraggableNodeEditor(getLocalBounds(), nodeTree,childWhichHasBeenAdded.getProperty(DraggableNodeIdentifiers::id));
    node->setParentBounds(this->getBounds());
    draggableNodes.emplace_back(node);
    addAndMakeVisible(node);
    node->setBounds(posX, posY, 10, 10);
}

void DynamicCurveEditor::valueTreeChildRemoved (juce::ValueTree& parentTree,juce::ValueTree& childWhichHasBeenRemoved,int indexFromWhichChildWasRemoved)
{
    jassertfalse;
}

void DynamicCurveEditor::valueTreeChildOrderChanged (juce::ValueTree& parentTreeWhoseChildrenHaveMoved,int oldIndex,int newIndex)
{
    jassertfalse;
}

void DynamicCurveEditor::valueTreeParentChanged (juce::ValueTree& treeWhoseParentHasChanged)
{
    jassertfalse;
}

juce::Point<float> DynamicCurveEditor::getPointFromNode(juce::ValueTree vt){
    
    auto x = vt.getProperty(DraggableNodeIdentifiers::posX);
    auto y = vt.getProperty(DraggableNodeIdentifiers::posY);
    
    return juce::Point<float>(x,y);
}
