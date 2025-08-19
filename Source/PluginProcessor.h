/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Backend/EnvelopeProcessor.h"
#include "Backend/Transport.h"
#include "Backend/CurveManager.h"
#include "Backend/PresetManager/PresetManager.h"

enum Divisions
{
  Eighth = 1,
  Quarter = 2,
  Half = 3,
  Whole = 4,
};

//==============================================================================
class SideChainAudioProcessor : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
{
public:
  //==============================================================================
  SideChainAudioProcessor();
  ~SideChainAudioProcessor() override;

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

  //==============================================================================
  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String &newName) override;

  //==============================================================================
  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;
  Service::PresetManager &getPresetManager() { return *presetManager; }
  CurveManager &getCurveManager() { return *curveManager; }

  float SideChainAudioProcessor::getRmsValue(const int channel) const;
  void callBack(std::vector<juce::Point<float>> d);

  juce::AudioProcessorValueTreeState &GetAPVTS() { return apvts; }

  void parameterChanged(const juce::String& parameterID, float newValue) override;

  EnvelopeProcessor envelopeProcessor;
  Transport transport;

private:
  LinearSmoothedValue<float> rmsLevelLeft, rmsLevelRight;
  std::unique_ptr<Service::PresetManager> presetManager;
  std::unique_ptr<CurveManager> curveManager;
  juce::AudioProcessorValueTreeState apvts;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SideChainAudioProcessor)
};
