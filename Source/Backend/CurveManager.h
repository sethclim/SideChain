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
    CurveManager()
    {
        nodes = juce::ValueTree(DraggableNodeIdentifiers::myRootDraggableTreeType);
    }

    void initializeCurveManager()
    {
        if (initialized)
            return;

        numberOfNodes = 0;
        nodes.addListener(this);
        addInitialNode(0, 0.5);
        addInitialNode(1, 0.5);
        calculateDataPointsFromTree();

        DBG(nodes.toXmlString());

        initialized = true;
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

    void insertNewNodeBetween(juce::Point<float> coord)
    {
        jassert(coord.x <= 1.0 && coord.y <= 1.0);

        if (!nodes.isValid())
            return;

        juce::ValueTree newNode(DraggableNodeIdentifiers::myNodeType);

        newNode.setProperty(DraggableNodeIdentifiers::posX, coord.x, nullptr);
        newNode.setProperty(DraggableNodeIdentifiers::posY, coord.y, nullptr);
        newNode.setProperty(DraggableNodeIdentifiers::id, numberOfNodes, nullptr);

        // get the right index to land the node between two other nodes
        // X ----- here ----- X //
        for (const auto &child : nodes)
        {
            const auto &child_X = child.getProperty(DraggableNodeIdentifiers::posX);
            auto childNext_X = child.getSibling(1).getProperty(DraggableNodeIdentifiers::posX);

            if ((int)child_X < coord.x && coord.x < (int)childNext_X)
            {
                auto childIdx = nodes.indexOf(child);

                nodes.addChild(newNode, childIdx + 1, nullptr);
                numberOfNodes++;
                break;
            }
        }

        if (redrawCallback)
            (redrawCallback)();
    }

    void moveNode(int id, juce::Point<float> coord) const
    {
        auto node = nodes.getChildWithProperty(DraggableNodeIdentifiers::id, id);

        if ((int)node.getProperty(DraggableNodeIdentifiers::id) == 0)
        {
            auto endNode = nodes.getChildWithProperty(DraggableNodeIdentifiers::id, 1);

            if (checkBetweenMinMax(coord.y, 0, 1))
            {
                node.setProperty(DraggableNodeIdentifiers::posY, coord.y, nullptr);
                endNode.setProperty(DraggableNodeIdentifiers::posY, coord.y, nullptr);
            }
        }
        else if ((int)node.getProperty(DraggableNodeIdentifiers::id) == 1)
        {
            auto startNode = nodes.getChildWithProperty(DraggableNodeIdentifiers::id, 0);
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

        for (const auto &child : nodes)
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
        if (!nodes.isValid())
            return;

        juce::ValueTree newNode(DraggableNodeIdentifiers::myNodeType);

        newNode.setProperty(DraggableNodeIdentifiers::posX, x, nullptr);
        newNode.setProperty(DraggableNodeIdentifiers::posY, y, nullptr);
        newNode.setProperty(DraggableNodeIdentifiers::id, numberOfNodes, nullptr);

        nodes.addChild(newNode, -1, nullptr);
        numberOfNodes++;

        DBG(nodes.toXmlString());

        if (redrawCallback)
            (redrawCallback)();
    }

    float limitValue(float value)
    {
        if (value > 1.0)
        {
            return 1.0;
        }
        else if (value < 0.0)
        {
            return 0.0;
        }

        return value;
    }

    bool checkBetweenMinMax(float value, float min, float max) const
    {
        return min <= value && value <= max;
    }

    std::vector<juce::Point<float>> segments;
    int numberOfNodes;
    EventCallback eventCallback;
    RedrawEvent redrawCallback;

public:
    int width;
    int height;
    int innerWidth;
    int innerHeight;
    juce::ValueTree nodes;
    int controlSize = 10;
    bool initialized = false;
};
