//
// Created by Seth Climenhaga on 2022-05-18.
//

#pragma once
#include <JuceHeader.h>

#include <utility>
#include "./DataPoint.h"
#include "../DraggableNodeIdentifiers.h"

// Set the width and height from the backend too.
typedef std::function<void(std::vector<juce::Point<float>>)> EventCallback;
typedef std::function<void(unsigned int id, juce::Point<float> position)> RedrawEvent;

class CurveManager : public juce::ValueTree::Listener
{
public:
    CurveManager()
    {
        nodes = juce::ValueTree(DraggableNodeIdentifiers::myRootDraggableTreeType);
    }

    void initializeCurveManager(int initWidth, int initHeight)
    {
        if (initialized)
            return;

        std::cout << "SETTING UP CURVE"
                  << "initWidth " << initWidth << "initHeight " << initHeight << std::endl;

        width = initWidth;
        height = initHeight;
        innerWidth = width - controlSize;
        innerHeight = height - controlSize;
        numberOfNodes = 0;
        nodes.addListener(this);
        addInitialNode(0, innerHeight / 2);
        addInitialNode(innerWidth, innerHeight / 2);
        calculateDataPointsFromTree();

        std::cout << "inner " << innerWidth << " " << innerHeight << std::endl;
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

    void insertNewNodeBetween(int x, int y)
    {
        if (!nodes.isValid())
            return;

        juce::ValueTree newNode(DraggableNodeIdentifiers::myNodeType);

        newNode.setProperty(DraggableNodeIdentifiers::posX, x, nullptr);
        newNode.setProperty(DraggableNodeIdentifiers::posY, y, nullptr);
        newNode.setProperty(DraggableNodeIdentifiers::id, numberOfNodes, nullptr);

        // get the right index to land the node between two other nodes
        // X ----- here ----- X //
        for (const auto &child : nodes)
        {
            const auto &child_X = child.getProperty(DraggableNodeIdentifiers::posX);
            auto childNext_X = child.getSibling(1).getProperty(DraggableNodeIdentifiers::posX);

            if ((int)child_X < x && x < (int)childNext_X)
            {
                auto childIdx = nodes.indexOf(child);

                nodes.addChild(newNode, childIdx + 1, nullptr);
                numberOfNodes++;
                break;
            }
        }

        // if (redrawCallback)
        //     (redrawCallback)(static_cast<unsigned int>(0), juce::Point<float>(0, 0));
    }

    void moveNode(int id, juce::Point<float> position) const
    {
        auto node = nodes.getChildWithProperty(DraggableNodeIdentifiers::id, id);
        int minWidth = 0;
        int maxWidth = innerWidth;
        int minHeight = 0;
        int maxHeight = innerHeight;

        if ((int)node.getProperty(DraggableNodeIdentifiers::id) == 0)
        {
            maxWidth = controlSize;

            auto endNode = nodes.getChildWithProperty(DraggableNodeIdentifiers::id, 1);

            if (checkBetweenMinMax(position.y, minHeight, maxHeight))
            {
                node.setProperty(DraggableNodeIdentifiers::posY, position.y, nullptr);
                endNode.setProperty(DraggableNodeIdentifiers::posY, position.y, nullptr);
            }
        }
        else if ((int)node.getProperty(DraggableNodeIdentifiers::id) == 1)
        {
            minWidth = innerWidth;
            auto startNode = nodes.getChildWithProperty(DraggableNodeIdentifiers::id, 0);
            if (checkBetweenMinMax(position.y, minHeight, maxHeight))
            {
                node.setProperty(DraggableNodeIdentifiers::posY, position.y, nullptr);
                startNode.setProperty(DraggableNodeIdentifiers::posY, position.y, nullptr);
            }
        }
        else
        {
            auto prevSib = node.getSibling(-1);
            auto nextSib = node.getSibling(1);

            if (prevSib.isValid() && nextSib.isValid())
            {
                minWidth = prevSib.getProperty(DraggableNodeIdentifiers::posX);
                maxWidth = nextSib.getProperty(DraggableNodeIdentifiers::posX);

                if (checkBetweenMinMax(position.x, minWidth, maxWidth))
                {
                    node.setProperty(DraggableNodeIdentifiers::posX, position.x, nullptr);
                }

                // if (static_cast<float>(minHeight) <= position.y && position.y <= static_cast<float>(maxHeight))
                if (checkBetweenMinMax(position.y, minHeight, maxHeight))
                {
                    node.setProperty(DraggableNodeIdentifiers::posY, position.y, nullptr);
                }
            }
        }

        // if (redrawCallback)
        //     (redrawCallback)(static_cast<unsigned int>(id), position);
    }

    void rescaleNodesWindowResized(int newWidth, int newHeight)
    {
        std::cout << "width " << width << " height " << height << std::endl;
        std::cout << "newWidth " << newWidth << " newHeight " << newHeight << std::endl;
        float scaleWidth = (float)newWidth / (float)width;
        float scaleHeight = (float)newHeight / (float)height;

        std::cout << "scalewidth " << scaleWidth << " scaleHeight " << scaleHeight << std::endl;

        for (const auto &child : nodes)
        {
            const float child_X = (float)child.getProperty(DraggableNodeIdentifiers::posX);
            const float child_Y = (float)child.getProperty(DraggableNodeIdentifiers::posY);

            float newX = child_X * scaleWidth;
            float newY = (float)newHeight - (child_Y * scaleHeight);

            juce::ValueTree &c = const_cast<juce::ValueTree &>(child);

            c.setProperty(DraggableNodeIdentifiers::posX, newX, nullptr);
            c.setProperty(DraggableNodeIdentifiers::posY, newY, nullptr);
        }

        DBG(nodes.toXmlString());

        width = width;
        height = height;
        innerWidth = newWidth - controlSize;
        innerHeight = newHeight - controlSize;
    }

private:
    void calculateDataPointsFromTree()
    {
        std::vector<juce::Point<float>> points = std::vector<juce::Point<float>>();

        for (const auto &child : nodes)
        {
            int id = (int)child.getProperty(DraggableNodeIdentifiers::id);
            float x = child.getProperty(DraggableNodeIdentifiers::posX);
            float y = child.getProperty(DraggableNodeIdentifiers::posY);

            float x_norm = x / innerWidth;
            float y_norm = juce::jmap<float>(y / innerHeight, 1.0, 0.0, 0.0, 1.0);

            x_norm = limitValue(x_norm);
            y_norm = limitValue(y_norm);

            auto point = juce::Point<float>(x_norm, y_norm);

            points.push_back(point);
        }

        if (eventCallback)
            (eventCallback)(points);

        segments = points;
    }

    void addInitialNode(int x, int y)
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

        // if (redrawCallback)
        //     (redrawCallback)(static_cast<unsigned int>(0), juce::Point<float>(0, 0));
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

    bool checkBetweenMinMax(float value, int min, int max) const
    {
        return static_cast<float>(min) <= value && value <= static_cast<float>(max);
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
