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
    
//    for(int i = 0; i < dynamicCurve.getNumberOfNodes() - 1;i++)
//    {
//        LineEditor* line = new LineEditor(juce::Point<float> (10.0f,10.0f), juce::Point<float> (50.0f,50.0f));
//        lines.emplace_back(line);
//        addAndMakeVisible(*lines[i]);
//
//    }
    
    int i = 0;
    
    //DBG(nodeTree.getNumChildren());
    
    for(const auto& child : nodeTree)
    {
        if(i < nodeTree.getNumChildren() - 1){
            //DBG("creating Line");
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

            //lines[idx]->setBounds(x, y, x2, y2);
        }
        
        idx++;
        jdx++;
    }
}

void DynamicCurveEditor::resized()
{
    
//    for(int i = 0; i < dynamicCurve.getNumberOfNodes() - 1;i++)
//    {
//        lines[i]->setBounds(draggableNodes[i]->getX(), draggableNodes[i]->getY(), 50, 50);
//    }
    
    
    auto zero = nodeTree.getChildWithProperty(DraggableNodeIdentifiers::id, 0);
    auto one = nodeTree.getChildWithProperty(DraggableNodeIdentifiers::id, 1);
    
    zero.setProperty(DraggableNodeIdentifiers::posY, this->getHeight() - 10, nullptr);
    
    std::cout<<"GEt width " << this->getWidth() << std::endl;
    one.setProperty(DraggableNodeIdentifiers::posX, this->getWidth() - 10, nullptr);
    one.setProperty(DraggableNodeIdentifiers::posY, this->getHeight() - 10, nullptr);
    
    int idx = 0;
    int jdx = 0;
    
    for(const auto& child : nodeTree)
    {
        auto x = child.getProperty(DraggableNodeIdentifiers::posX);
        auto y = child.getProperty(DraggableNodeIdentifiers::posY);
        
        
//        if(jdx < numNodes - 1){
//            auto x2 = child.getSibling(1).getProperty(DraggableNodeIdentifiers::posX);
//            auto y2 = child.getSibling(1).getProperty(DraggableNodeIdentifiers::posY);
//
//            lines[idx]->setBounds(x, y, x2, y2);
//        }
        
        draggableNodes[idx]->setBounds(x ,y, 10, 10);
        draggableNodes[idx]->setParentBounds(this->getBounds());
        
        // set Bounds here so they know the new min max width
        
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

void DynamicCurveEditor::mouseDrag (const juce::MouseEvent& event)
{
    
//    std::cout<<"Mouse X" << event.x << " Mouse Y " << event.y << std::endl;
    
    
}

//void DynamicCurveEditor::addNewNodeCallbackHandler(int childIndex)
//{
//    int numNodes = nodeTree.getProperty(DraggableNodeIdentifiers::numberOfNodes);
//
//    for(int i = 0; i < numNodes;i++)
//    {
//        DraggableNodeEditor* node = new DraggableNodeEditor(getLocalBounds());
//        addAndMakeVisible(node);
//        node->setBounds(i * 10, 0, 10, 10);
//
//    }
//
//}

void DynamicCurveEditor::valueTreePropertyChanged (juce::ValueTree& nodeChanged, const juce::Identifier& property)
{
    //Set up around 2 nodes and one line for now
    
    //2nd node at end of line
//    if(nodeChanged.getSibling(-1).isValid()){
//
//        int lineIDX = (int) nodeChanged.getProperty(DraggableNodeIdentifiers::id) - 1;
//
//        juce::Point<float> start = getPointFromNode(nodeChanged.getSibling(-1));
//        juce::Point<float> end = getPointFromNode(nodeChanged);
//
//
//        lines[lineIDX]->setBounds(start.x, start.y, end.x - start.x , end.y - start.y);
//
//        printf("2nd Node XY (%0.2f,%0.2f)\n", end.x, end.y);
//
//        printf("Bounds of line ");
//        DBG(lines[lineIDX]->getBounds().toString());
//    }
//    // first node at start of line
//    else if(nodeChanged.getSibling(1).isValid()){
//
//        int lineIDX = (int) nodeChanged.getProperty(DraggableNodeIdentifiers::id);
//
//        juce::Point<float> start = getPointFromNode(nodeChanged);
//        juce::Point<float> end = getPointFromNode(nodeChanged.getSibling(1));
//
//        printf("1st Node XY (%0.2f,%0.2f)\n", start.x, start.y);
//
//        lines[lineIDX]->setBounds(start.x, start.y, end.x - start.x, end.y - start.y);
//
//        printf("Bounds of line");
//        DBG(lines[lineIDX]->getBounds().toString());
//    }
   //addNewNodeCallbackHandler(nodeTree.indexOf(treeWhosePropertyHasChanged));
    
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
