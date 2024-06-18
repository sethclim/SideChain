//
// Created by Seth Climenhaga on 2022-05-18.
//

#pragma once
#include <JuceHeader.h>
#include <utility>
#include "../DraggableNodeIdentifiers.h"

// Set the width and height from the backend too.
typedef std::function<void(std::vector<juce::Point<float>>)> EventCallback;
typedef std::function<void()> RedrawEvent;

class CurveManager : public juce::ValueTree::Listener
{
public:
    CurveManager(juce::AudioProcessorValueTreeState &apvts) : apvts(apvts), numberOfNodes(0)
    {
        numberOfNodes = 0;
        apvts.state.addListener(this);

        if (!apvts.state.getChildWithName(DraggableNodeIdentifiers::myRootDraggableTreeType).isValid())
        {
            DBG("NOT IS VALID!!!!!!!!!!!");
            juce::ValueTree &root = juce::ValueTree(DraggableNodeIdentifiers::myRootDraggableTreeType);

            apvts.state.appendChild(root, nullptr);

            addInitialNode(0, 0.5);
            addInitialNode(1, 0.5);
        }

        calculateDataPointsFromTree();
    }

    void registerOnCalculateDataPointsCallback(EventCallback cb) { eventCallback = std::move(cb); }

    void registerOnMoveNodeCallback(RedrawEvent redrawEvent) { redrawCallback = std::move(redrawEvent); }

    void valueTreePropertyChanged(juce::ValueTree &nodeChanged, const juce::Identifier &property) override
    {
        calculateDataPointsFromTree();
    }

    std::vector<juce::Point<float>> getDataPoints()
    {
        return segments;
    }

    void insertNewNodeBetween(juce::Point<float> inComingCoord)
    {
        if (!apvts.state.getChildWithName(DraggableNodeIdentifiers::myRootDraggableTreeType).isValid())
            return;

        inComingCoord = limitValue(inComingCoord);

        juce::ValueTree newNode(DraggableNodeIdentifiers::myNodeType);

        newNode.setProperty(DraggableNodeIdentifiers::posX, inComingCoord.x, nullptr);
        newNode.setProperty(DraggableNodeIdentifiers::posY, inComingCoord.y, nullptr);
        newNode.setProperty(DraggableNodeIdentifiers::id, numberOfNodes, nullptr);

        // get the right index to land the node between two other nodes
        // X ----- here ----- X //
        for (const auto &child : apvts.state.getChildWithName(DraggableNodeIdentifiers::myRootDraggableTreeType))
        {
            const auto &child_X = child.getProperty(DraggableNodeIdentifiers::posX);
            auto childNext_X = child.getSibling(1).getProperty(DraggableNodeIdentifiers::posX);

            if ((float)child_X < inComingCoord.x && inComingCoord.x < (float)childNext_X)
            {
                auto childIdx = apvts.state.getChildWithName(DraggableNodeIdentifiers::myRootDraggableTreeType).indexOf(child);

                apvts.state.getChildWithName(DraggableNodeIdentifiers::myRootDraggableTreeType).addChild(newNode, childIdx + 1, nullptr);
                numberOfNodes++;
                break;
            }
        }

        if (redrawCallback)
            (redrawCallback)();
    }

    void moveNode(int id, juce::Point<float> coord) const
    {
        auto node = apvts.state.getChildWithName(DraggableNodeIdentifiers::myRootDraggableTreeType).getChildWithProperty(DraggableNodeIdentifiers::id, id);

        if ((int)node.getProperty(DraggableNodeIdentifiers::id) == 0)
        {
            auto endNode = apvts.state.getChildWithName(DraggableNodeIdentifiers::myRootDraggableTreeType).getChildWithProperty(DraggableNodeIdentifiers::id, 1);

            if (checkBetweenMinMax(coord.y, 0, 1))
            {
                node.setProperty(DraggableNodeIdentifiers::posY, coord.y, nullptr);
                endNode.setProperty(DraggableNodeIdentifiers::posY, coord.y, nullptr);
            }
        }
        else if ((int)node.getProperty(DraggableNodeIdentifiers::id) == 1)
        {
            auto startNode = apvts.state.getChildWithName(DraggableNodeIdentifiers::myRootDraggableTreeType).getChildWithProperty(DraggableNodeIdentifiers::id, 0);
            if (checkBetweenMinMax(coord.y, 0, 1))
            {
                node.setProperty(DraggableNodeIdentifiers::posY, coord.y, nullptr);
                startNode.setProperty(DraggableNodeIdentifiers::posY, coord.y, nullptr);
            }
        }
        else
        {
            auto prevSib = node.getSibling(-1);
            auto nextSib = node.getSibling(1);

            if (prevSib.isValid() && nextSib.isValid())
            {
                float minWidth = prevSib.getProperty(DraggableNodeIdentifiers::posX);
                float maxWidth = nextSib.getProperty(DraggableNodeIdentifiers::posX);

                if (checkBetweenMinMax(coord.x, minWidth, maxWidth))
                {
                    node.setProperty(DraggableNodeIdentifiers::posX, coord.x, nullptr);
                }

                if (checkBetweenMinMax(coord.y, 0, 1))
                {
                    node.setProperty(DraggableNodeIdentifiers::posY, coord.y, nullptr);
                }
            }
        }

        if (redrawCallback)
            (redrawCallback)();
    }

private:
    void calculateDataPointsFromTree()
    {
        std::vector<juce::Point<float>> points = std::vector<juce::Point<float>>();

        for (const auto &child : apvts.state.getChildWithName(DraggableNodeIdentifiers::myRootDraggableTreeType))
        {
            float x = child.getProperty(DraggableNodeIdentifiers::posX);
            float y = child.getProperty(DraggableNodeIdentifiers::posY);

            float y_inv = juce::jmap<float>(y, 1.0, 0.0, 0.0, 1.0);

            auto point = juce::Point<float>(x, y_inv);

            points.push_back(point);
        }

        if (eventCallback)
            (eventCallback)(points);

        segments = points;
    }

    void addInitialNode(float x, float y)
    {
        if (!apvts.state.getChildWithName(DraggableNodeIdentifiers::myRootDraggableTreeType).isValid())
            return;

        juce::ValueTree newNode(DraggableNodeIdentifiers::myNodeType);

        newNode.setProperty(DraggableNodeIdentifiers::posX, x, nullptr);
        newNode.setProperty(DraggableNodeIdentifiers::posY, y, nullptr);
        newNode.setProperty(DraggableNodeIdentifiers::id, numberOfNodes, nullptr);

        apvts.state.getChildWithName(DraggableNodeIdentifiers::myRootDraggableTreeType).addChild(newNode, -1, nullptr);
        numberOfNodes++;

        DBG(apvts.state.getChildWithName(DraggableNodeIdentifiers::myRootDraggableTreeType).toXmlString());

        if (redrawCallback)
            (redrawCallback)();
    }

    juce::Point<float> limitValue(juce::Point<float> value)
    {
        if (value.x > 1.0)
        {
            value.x = 1.0;
        }
        else if (value.x < 0.0)
        {
            value.x = 0.0;
        }

        if (value.y > 1.0)
        {
            value.y = 1.0;
        }
        else if (value.y < 0.0)
        {
            value.y = 0.0;
        }

        return value;
    }

    bool checkBetweenMinMax(float value, float min, float max) const
    {
        return min <= value && value <= max;
    }

    void valueTreeRedirected(juce::ValueTree &treeWhichHasBeenChanged)
    {
        calculateDataPointsFromTree();
    }

    std::vector<juce::Point<float>> segments;
    int numberOfNodes = 0;
    EventCallback eventCallback;
    RedrawEvent redrawCallback;
    juce::AudioProcessorValueTreeState &apvts;

public:
    int controlSize = 10;
};
