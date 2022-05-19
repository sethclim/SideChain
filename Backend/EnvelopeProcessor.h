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

    explicit EnvelopeProcessor(Transport& t) : transport(t)
    {
        juce::ValueTree myNodes (DraggableNodeIdentifiers::myRootDraggableTreeType);
    }

    ~EnvelopeProcessor() = default;

    void setSideChainEnv(std::vector<DataPoint> dataPoints){
        segments = std::move(dataPoints);
    }

    double getNextSample() noexcept
    {
        if(relativePosition == 0.0){
            idx = 0;
        }

        if(!segments.empty())
        {
            if(relativePosition > segments[idx].end )
                idx++;

            double vol = (segments[idx].slope * (relativePosition - segments[idx].start)) + segments[idx].y_intercept;

            if(vol > 1.0){
                vol = 1.0;
            }

            return vol;
        }
        else{
            return 0.0;
        }
    }

    void ApplySideChainToBuffer(juce::AudioBuffer<float>& buffer, int startSample, int numSamples){
        
        auto numChannels = buffer.getNumChannels();
        
        while (--numSamples >= 0)
        {
            auto vol = getNextSample();
            currentVol.store(vol);
            relativePosition = fmod(transport.ppqPositions[static_cast<unsigned long>(startSample)], 1.0);
            relPosition.store(relativePosition);
            
            for (int i = 0; i < numChannels; ++i)
                buffer.getWritePointer (i)[startSample] *= (float) vol;

            ++startSample;
        }
    }

    std::vector<DataPoint> segments;
    std::atomic<double> currentVol {0};
    std::atomic<double> relPosition {0};
    
private:
    Transport& transport;
    double relativePosition  = 0.0;
    unsigned int idx = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeProcessor)
};
