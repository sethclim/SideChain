/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SideChainAudioProcessor::SideChainAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), dynamicCurve(transport)
#endif
{
}

SideChainAudioProcessor::~SideChainAudioProcessor()
{
}

//==============================================================================
const juce::String SideChainAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SideChainAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SideChainAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SideChainAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SideChainAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SideChainAudioProcessor::getNumPrograms()
{
    // NB: some hosts don't cope very well with 0 programs,
    // so this should be at least 1;
    return 1;
               
}

int SideChainAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SideChainAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SideChainAudioProcessor::getProgramName (int index)
{
    return {};
}

void SideChainAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SideChainAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    transport.prepare(sampleRate, samplesPerBlock);
}

void SideChainAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SideChainAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


void SideChainAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    
    transport.process(getPlayHead(), buffer.getNumSamples());
    
    dynamicCurve.ApplySideChainToBuffer(buffer, 0,buffer.getNumSamples());
}


//==============================================================================
bool SideChainAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SideChainAudioProcessor::createEditor()
{
    return new SideChainAudioProcessorEditor (*this);
}

//==============================================================================
void SideChainAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SideChainAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SideChainAudioProcessor();
}
