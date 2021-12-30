/*
  ==============================================================================

    DynamicCurveEditor.cpp
    Created: 16 May 2021 11:37:16pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#include "DynamicCurve.h"
#include "DynamicCurveEditor.h"


DynamicCurve::DynamicCurve()
{
    numberOfNodes = 0;
    
    juce::ValueTree myNodes (DraggableNodeIdentifiers::myRootDraggableTreeType);
    draggableNodes = myNodes;
    
    addNewNode(10,10);
    addNewNode(100,100);
    
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
        
        //DBG(draggableNodes.toXmlString());
        
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
            float x1 = (float)child.getProperty(DraggableNodeIdentifiers::posX);
            float y1 = (float)child.getProperty(DraggableNodeIdentifiers::posY);
            
            float x2 = (float)child.getSibling(1).getProperty(DraggableNodeIdentifiers::posX);
            float y2 = (float)child.getSibling(1).getProperty(DraggableNodeIdentifiers::posY);
            
            float x1Pos = x1 / width;
            float x2Pos = x2 / width;
            
            float y1Pos = juce::jmap<float>(y1 / height, 1.0, 0.0, 0.0, 1.0);
            float y2Pos = juce::jmap<float>(y2 / height, 1.0, 0.0, 0.0, 1.0);
            
            float slope = std::abs(y2Pos - y1Pos) / std::abs(x2Pos - x1Pos);
            
            
            //std::cout << "(x1,y1) (" << x1Pos <<","<< y1Pos << ")" << "(x2,y2) (" << x2Pos <<","<< y2Pos << ")" << std::endl;
            
            //std::cout << "slope " << slope << std::endl;
            
            float yIntercept =  y2Pos - (slope * x2Pos);
            
            
            //std::cout << "yIntercept " << yIntercept << " = " << y2Pos << "- (" << slope <<" * " << x2Pos << ")" << std::endl;
            
            // start %, end %, slope, y-intercept
            auto segment = DataPoint(x1Pos, x2Pos, slope, yIntercept);
            
            segments.push_back(segment);
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

int idx = 0;
float currWhole = -1.0;


void DynamicCurve::ProcessAudio(float* channelData, int numSamples, juce::AudioPlayHead::CurrentPositionInfo& hostInfo){
    
    float whole, frac_BeatPos = 0.0;
    
    auto currentPos = hostInfo.ppqPosition;
    frac_BeatPos = std::modf(currentPos, &whole);
    
    
    float vol = 1.0;
    
    if(whole != currWhole){
        idx = 0;
        currWhole = whole;
    }
    
    if(segments.size() > 0){
       //std::cout <<"POS: " << frac_BeatPos << " start,end: (" << segments[idx].start << "," <<  segments[idx].end<< ")"<< "IDX: " << idx << " yintercept "<< segments[idx].yintercept << std::endl;
        
        if(frac_BeatPos >= segments[idx].start && frac_BeatPos < segments[idx].end)
        {
            vol = (frac_BeatPos * segments[idx].slope) + segments[idx].yintercept;
            
           // DBG(vol);
        }
//        else{
//            if(idx <= segments.size())
//                idx++;
//        }
        //std::cout <<"Vol: " << vol << " = " << frac_BeatPos << " * " <<  points[idx].slope<< " + " << points[idx].yintercept << std::endl;
    }
    
    //Process applied per sample in buffer
    for (int sample = 0; sample < numSamples; ++sample)
    {
        channelData[sample] = channelData[sample] *  vol;
    }
}
