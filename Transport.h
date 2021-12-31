/*
  ==============================================================================

    Transport.h
    Created: 30 Dec 2021 9:02:30pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#pragma once

struct Transport
{
    void prepare(double sampleRate, int maxBlockSize)
    {
        sampleInMinute = sampleRate * 60.0;
        ppqPositions.resize(maxBlockSize);
    }
    
    double getPPQPerSample() const
    {
        auto samplesPerBeat = sampleInMinute / info.bpm;
        return 1.0 / samplesPerBeat;
    }
    
    void process(juce::AudioPlayHead* playHead, int numSamples)
    {
        if(playHead != nullptr)
            playHead->getCurrentPosition(info);
        
        auto ppqPerSample = getPPQPerSample();
        double offset = 0.0;
        
        for(int sample = 0; sample < numSamples; ++sample)
        {
            ppqPositions[sample] = info.ppqPosition + offset;
            offset += ppqPerSample;
        }
        //currentPosition.store
    }
    
    std::atomic<double> currentPosition {0};
    double sampleInMinute = 0.0;
    juce::AudioPlayHead::CurrentPositionInfo info;
    std::vector<double> ppqPositions;
};
