/*
  ==============================================================================

    DynamicCurveEditor.cpp
    Created: 16 May 2021 11:37:16pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#include "DynamicCurve.h"
#include "DynamicCurveEditor.h"


DynamicCurve::DynamicCurve(Transport& t) : transport(t)
{
    numberOfNodes = 0;
    
    juce::ValueTree myNodes (DraggableNodeIdentifiers::myRootDraggableTreeType);
    draggableNodes = myNodes;
    
    addNewNode(0,100);
    addNewNode(300,100);
    
}
DynamicCurve::~DynamicCurve(){
 
}

juce::Component* DynamicCurve::createEditor()
{
    return new DynamicCurveEditor(*this, draggableNodes);
}

int DynamicCurve::getNumberOfNodes(){
    return numberOfNodes;
}
void DynamicCurve::addNewNode(float x, float y){
    
    DraggableNode* dNode = new DraggableNode(x,y);

    //nodes.emplace_back(dNode);
    
    if (draggableNodes.isValid())
    {
        juce::ValueTree myNode (DraggableNodeIdentifiers::myNodeType);
        
        
        myNode.setProperty(DraggableNodeIdentifiers::posX, dNode->getX(), nullptr);
        myNode.setProperty(DraggableNodeIdentifiers::posY, dNode->getY(), nullptr);
        myNode.setProperty(DraggableNodeIdentifiers::id, numberOfNodes, nullptr);
        
        // get the right index to land the node between two other nodes
        
        bool slotFound = false;
        
        // X ----- here ----- X //
        for(const auto& child : draggableNodes)
        {
            auto child_X = child.getProperty(DraggableNodeIdentifiers::posX);
            auto childNext_X = child.getSibling(1).getProperty(DraggableNodeIdentifiers::posX);
            
            if((float)child_X < x && x < (float)childNext_X)
            {
               auto childIdx =  draggableNodes.indexOf(child);
                
                draggableNodes.addChild(myNode, childIdx + 1, nullptr);
                numberOfNodes++;
                slotFound = true;
                break;
            }
        }
        
        if(!slotFound)
        {
            draggableNodes.addChild(myNode, -1, nullptr);
            numberOfNodes++;
        }
    }
}

void DynamicCurve::calculateDataPointsFromTree(float width, float height){
    
    //std::cout<< "CalculateDataPointsFromTree() called"<< std::endl;
    
    segments.clear();
    
    for(const auto& child : draggableNodes)
    {
        if(child.getSibling(1).isValid())
        {
            int id = (int)child.getProperty(DraggableNodeIdentifiers::id);
            float x1 = (float)child.getProperty(DraggableNodeIdentifiers::posX);
            float y1 = (float)child.getProperty(DraggableNodeIdentifiers::posY);
            
            float x2 = (float)child.getSibling(1).getProperty(DraggableNodeIdentifiers::posX);
            float y2 = (float)child.getSibling(1).getProperty(DraggableNodeIdentifiers::posY);
            
            float x1Pos = x1  / (width - 10);
            float x2Pos = x2  / (width - 10);
            
            float y1Pos = juce::jmap<float>(y1  / (height  - 10) , 1.0, 0.0, 0.0, 1.0);
            float y2Pos = juce::jmap<float>(y2 / (height - 10), 1.0, 0.0, 0.0, 1.0);
            
            float slope = (y2Pos - y1Pos) / (x2Pos - x1Pos);
            
            std::cout << "---------------------\n(x1,y1) (" << x1Pos <<","<< y1Pos << ")" << "(x2,y2) (" << x2Pos <<","<< y2Pos << ")" << std::endl;
            std::cout << "slope " << slope << std::endl;
            
            float yIntercept =  y2Pos - (slope * x2Pos);
            
            std::cout << "yIntercept " << yIntercept << " = " << y2Pos << "- (" << slope <<" * " << x2Pos << ")" << std::endl;
            // start %, end %, slope, y-intercept
            auto segment = DataPoint(x1Pos, x2Pos, slope, yIntercept);
            
            segments.push_back(segment);
            
            std::cout<<"Segment id: " << id << " start " << segment.start << " end " <<segment.end << " slope " <<segment.slope << " YInercept " <<segment.yintercept<<std::endl;
        }
        else
        {
            //DBG("Bad Child");
        }
    }
}

std::vector<DataPoint> DynamicCurve::getSegments(){
    return segments;
}


float DynamicCurve::getNextSample() noexcept
{
    if(relativePosition == 0.0){
        idx = 0;
    }
    
    if(relativePosition > segments[idx].end )
        idx++;
    
    float vol = (segments[idx].slope * (relativePosition - segments[idx].start)) + segments[idx].yintercept;
    
    if(vol > 1.0){
        vol = 1.0;
    }
    
    return vol;
}


void DynamicCurve::ApplySideChainToBuffer(juce::AudioBuffer<float>& buffer, int startSample, int numSamples){
    
    auto numChannels = buffer.getNumChannels();
    
    while (--numSamples >= 0)
    {
        auto vol = getNextSample();
        currentVol.store(vol);
        relativePosition = fmod(transport.ppqPositions[startSample], 1.0);
        relPosition.store(relativePosition);
        
        for (int i = 0; i < numChannels; ++i)
            buffer.getWritePointer (i)[startSample] *= vol;

        ++startSample;
    }
}
