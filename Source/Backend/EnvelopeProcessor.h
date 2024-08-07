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
#include "Transport.h"
#include "RealtimeObject/RealtimeObject.hpp"

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
        farbot::RealtimeObject<std::vector<juce::Point<float>>, farbot::RealtimeObjectOptions::nonRealtimeMutatable>::ScopedAccess<farbot::ThreadType::nonRealtime> liveSegments(segments);
        *liveSegments = dataPoints;
    }

    double getNextSample(double relativePosition, std::vector<juce::Point<float>> points) noexcept
    {
        if (points.empty())
            return 0.0;

        unsigned int newSize = points.size();
        if (currentPointsSize == 0)
        {
            currentPointsSize = newSize;
        }
        else if (idx != 0 && newSize != currentPointsSize)
        {
            float prevRelPos = idx / currentPointsSize;
            idx = newSize * prevRelPos;
            currentPointsSize = newSize;
        }

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
        farbot::RealtimeObject<std::vector<juce::Point<float>>, farbot::RealtimeObjectOptions::nonRealtimeMutatable>::ScopedAccess<farbot::ThreadType::realtime> liveSegments(segments);

        while (--numSamples >= 0)
        {
            double relativePosition = fmod(transport.ppqPositions[static_cast<unsigned long>(startSample)] * divisions, 1.0);
            auto vol = getNextSample(relativePosition, *liveSegments);
            currentVol.store(vol);
            relPosition.store(relativePosition);

            for (int i = 0; i < numChannels; ++i)
            {
                buffer.getWritePointer(i)[startSample] *= (float)vol;
            }

            ++startSample;
        }
    }

    // std::vector<juce::Point<float>> Segments;
    farbot::RealtimeObject<std::vector<juce::Point<float>>, farbot::RealtimeObjectOptions::nonRealtimeMutatable> segments;

    std::atomic<double> currentVol{0};
    std::atomic<double> relPosition{0};

    void SetDivisions(float newDivision) { divisions = newDivision; }

private:
    Transport &transport;
    unsigned int idx = 0;
    float divisions = 1;

    unsigned int currentPointsSize = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeProcessor)
};
