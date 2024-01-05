/*
  ==============================================================================

    DynamicCurveEditor.h
    Created: 16 May 2021 11:37:16pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <utility>
#include "../DraggableNodeIdentifiers.h"
#include "DataPoint.h"
#include "Transport.h"

class EnvelopeProcessor
{
public:
    explicit EnvelopeProcessor(Transport &t) : transport(t)
    {
        juce::ValueTree myNodes(DraggableNodeIdentifiers::myRootDraggableTreeType);
    }

    ~EnvelopeProcessor() = default;

    void setSideChainEnv(std::vector<juce::Point<float>> dataPoints)
    {
        segments = std::move(dataPoints);
    }

    double getNextSample(double relativePosition, std::vector<juce::Point<float>> points) noexcept
    {
        if (points.empty())
            return 0.0;

        if (relativePosition < points[1].x)
            idx = 0;

        if (relativePosition >= points[idx + 1].x)
            idx++;

        double slope = (points[idx + 1].y - points[idx].y) / (points[idx + 1].x - points[idx].x);
        float yIntercept = points[idx].y - (slope * points[idx].x);

        double vol = (slope * relativePosition) + yIntercept;

        if (vol > 1.0)
            vol = 1.0;

        return vol;
    }

    void ApplySideChainToBuffer(juce::AudioBuffer<float> &buffer, int startSample, int numSamples)
    {
        auto numChannels = buffer.getNumChannels();

        while (--numSamples >= 0)
        {
            double relativePosition = fmod(transport.ppqPositions[static_cast<unsigned long>(startSample)], 1.0);
            auto vol = getNextSample(relativePosition, segments);
            currentVol.store(vol);
            relPosition.store(relativePosition);

            for (int i = 0; i < numChannels; ++i)
            {
                buffer.getWritePointer(i)[startSample] *= (float)vol;
            }

            ++startSample;
        }
    }

    std::vector<juce::Point<float>> segments;
    std::atomic<double> currentVol{0};
    std::atomic<double> relPosition{0};

private:
    Transport &transport;
    unsigned int idx = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeProcessor)
};