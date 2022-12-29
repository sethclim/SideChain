//
// Created by Seth Climenhaga on 2022-05-18.
//

#pragma once
#include <JuceHeader.h>

#include <utility>
#include "../DraggableNodeIdentifiers.h"
#include "./DataPoint.h"


// Set the width and height from the backend too.
typedef std::function<void (std::vector<DataPoint> )> EventCallback;
typedef std::function<void (unsigned int id, juce::Point<float> position)> RedrawEvent;

class CurveManager  : public juce::ValueTree::Listener
{
public:

    CurveManager(int initWidth, int initHeight)
    {
        nodes = juce::ValueTree(DraggableNodeIdentifiers::myRootDraggableTreeType);
        width = initWidth;
        height = initHeight;
        numberOfNodes = 0;
        nodes.addListener(this);
        addNewNode(0,0);
        addNewNode(width - 10, height - 10);
    }

    void registerOnCalculateDataPointsCallback(EventCallback cb){ eventCallback = std::move(cb); }

    void registerOnMoveNodeCallback(RedrawEvent redrawEvent){ redrawCallback = std::move(redrawEvent); }

    void valueTreePropertyChanged (juce::ValueTree& nodeChanged, const juce::Identifier& property) override
    {
        calculateDataPointsFromTree();
    }

    std::vector<DataPoint> getDataPoints(){
        return segments;
    }

    void addNewNode(int x, int y){

        if (nodes.isValid())
        {
            juce::ValueTree newNode (DraggableNodeIdentifiers::myNodeType);

            newNode.setProperty(DraggableNodeIdentifiers::posX, x, nullptr);
            newNode.setProperty(DraggableNodeIdentifiers::posY, y, nullptr);
            newNode.setProperty(DraggableNodeIdentifiers::id, numberOfNodes, nullptr);

            // get the right index to land the node between two other nodes

            bool slotFound = false;

            // X ----- here ----- X //
            for(const auto& child : nodes)
            {
                const auto& child_X = child.getProperty(DraggableNodeIdentifiers::posX);
                auto childNext_X = child.getSibling(1).getProperty(DraggableNodeIdentifiers::posX);

                if((int)child_X < x && x < (int)childNext_X)
                {
                    auto childIdx =  nodes.indexOf(child);

                    nodes.addChild(newNode, childIdx + 1, nullptr);
                    numberOfNodes++;
                    slotFound = true;
                    break;
                }
            }

            if(!slotFound)
            {
                nodes.addChild(newNode, -1, nullptr);
                numberOfNodes++;
            }

            if (redrawCallback)
            {
                (redrawCallback)(static_cast<unsigned int>(0), juce::Point<float>(0,0));
            }
        }
    }

    void moveNode(int id, juce::Point<float> position) const{
        auto node = nodes.getChildWithProperty(DraggableNodeIdentifiers::id, id);
        int minWidth = 0;
        int maxWidth = width;
        int minHeight = 0;
        int maxHeight = height - 10;

        if((int)node.getProperty(DraggableNodeIdentifiers::id) == 0)
        {
            maxWidth = 10;
        }
        else if((int)node.getProperty(DraggableNodeIdentifiers::id) == 1)
        {
            minWidth = width - 10 - 10;
        }
        else
        {
            auto prevSib = node.getSibling(-1);
            auto nextSib = node.getSibling(1);

            if(prevSib.isValid()){
                minWidth = prevSib.getProperty(DraggableNodeIdentifiers::posX);
            }

            if(nextSib.isValid()){
                maxWidth = nextSib.getProperty(DraggableNodeIdentifiers::posX);
            }
        }

        //node.setProperty(DraggableNodeIdentifiers::posX, position.x, nullptr);
        //node.setProperty(DraggableNodeIdentifiers::posY, position.y, nullptr);
        //std::cout<< " X  " << (int)x <<" y " << (int)y << std::endl;

        //int final_x = s_x, final_y = s_y;

        if(static_cast<float>(minWidth) <= position.x && position.x <= static_cast<float>(maxWidth))
        {
            node.setProperty(DraggableNodeIdentifiers::posX, position.x, nullptr);
        }

        if(static_cast<float>(minHeight) <= position.y && position.y <= static_cast<float>(maxHeight))
        {
            node.setProperty(DraggableNodeIdentifiers::posY, position.y, nullptr);
        }
        if (redrawCallback)
        {
            (redrawCallback)(static_cast<unsigned int>(id), position);
        }
    }



private:

    void calculateDataPointsFromTree(){
        std::vector<DataPoint> newSegments = std::vector<DataPoint>();

        for(const auto& child : nodes)
        {
            if(child.getSibling(1).isValid())
            {
                int id = (int)child.getProperty(DraggableNodeIdentifiers::id);
                int x1 = child.getProperty(DraggableNodeIdentifiers::posX);
                int y1 = child.getProperty(DraggableNodeIdentifiers::posY);

                int x2 = child.getSibling(1).getProperty(DraggableNodeIdentifiers::posX);
                int y2 = child.getSibling(1).getProperty(DraggableNodeIdentifiers::posY);

                int x1Pos = x1  / (width - 10);
                int x2Pos = x2  / (width - 10);

                auto y1Pos = juce::jmap<int>(y1  / (height  - 10) , 1.0, 0.0, 0.0, 1.0);
                auto y2Pos = juce::jmap<int>(y2 / (height - 10), 1.0, 0.0, 0.0, 1.0);

                float slope = (float)(y2Pos - y1Pos) / (float)(x2Pos - x1Pos);

                //std::cout << "---------------------\n(x1,y1) (" << x1Pos <<","<< y1Pos << ")" << "(x2,y2) (" << x2Pos <<","<< y2Pos << ")" << std::endl;
               // std::cout << "slope " << slope << std::endl;

                float yIntercept = static_cast<float>(y2Pos) - (slope * static_cast<float>(x2Pos));

                //std::cout << "yIntercept " << yIntercept << " = " << y2Pos << "- (" << slope <<" * " << x2Pos << ")" << std::endl;
                // start %, end %, slope, y-intercept
                auto segment = DataPoint(x1Pos, x2Pos, slope, yIntercept);

                newSegments.push_back(segment);

                //std::cout << "Segment id: " << id << " start " << segment.start << " end " << segment.end << " slope " << segment.slope << " YInercept " << segment.y_intercept << std::endl;
            }
        }

        if (eventCallback)(eventCallback)(newSegments);

        segments = newSegments;
    }

    std::vector<DataPoint> segments;
    int numberOfNodes;
    EventCallback eventCallback;
    RedrawEvent redrawCallback;

public:
    int width;
    int height;
    juce::ValueTree nodes;
};
