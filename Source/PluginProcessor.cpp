/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include <utility>
#include "PluginEditor.h"
// #include "JucePluginDefines.h"

AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    AudioProcessorValueTreeState::ParameterLayout layout;

    for (int i = 1; i < 2; ++i)
        layout.add(std::make_unique<AudioParameterInt>(String(i), String(i), 0, i, 0));

    return layout;
}

//==============================================================================
SideChainAudioProcessor::SideChainAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                         ),
#endif
      apvts(*this, nullptr, "PARAMETERS", createParameterLayout()),
      envelopeProcessor(transport),
      curveManager(apvts),
      presetManager(apvts)
{
    curveManager.registerOnCalculateDataPointsCallback([this](std::vector<juce::Point<float>> dataPoints)
                                                       { envelopeProcessor.setSideChainEnv(std::move(dataPoints)); });

    presetManager.BeginListening();
}

SideChainAudioProcessor::~SideChainAudioProcessor() = default;

//==============================================================================
const juce::String SideChainAudioProcessor::getName() const
{
    // return JucePlugin_Name;
    return "SIDECHAIN";
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

void SideChainAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String SideChainAudioProcessor::getProgramName(int index)
{
    return {};
}

void SideChainAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
}

//==============================================================================
void SideChainAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    transport.prepare(sampleRate, samplesPerBlock);
    rmsLevelLeft.reset(sampleRate, 0.5);
    rmsLevelRight.reset(sampleRate, 0.5);
}

void SideChainAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SideChainAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif q

void SideChainAudioProcessor::callBack(std::vector<juce::Point<float>> d)
{
    DBG("Callback triggered");

    envelopeProcessor.setSideChainEnv(d);
}

void SideChainAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    rmsLevelLeft.skip(buffer.getNumSamples());
    rmsLevelRight.skip(buffer.getNumSamples());
    {
        const auto value = Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumChannels()));
        if (value < rmsLevelLeft.getCurrentValue())
            rmsLevelLeft.setTargetValue(value);
        else
            rmsLevelLeft.setCurrentAndTargetValue(value);
    }

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    transport.process(getPlayHead(), buffer.getNumSamples());
    envelopeProcessor.ApplySideChainToBuffer(buffer, 0, buffer.getNumSamples());
    {
        const auto value = Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumChannels()));
        if (value < rmsLevelRight.getCurrentValue())
            rmsLevelRight.setTargetValue(value);
        else
            rmsLevelRight.setCurrentAndTargetValue(value);
    }
}

//==============================================================================
bool SideChainAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor *SideChainAudioProcessor::createEditor()
{
    return new SideChainAudioProcessorEditor(*this);
}

//==============================================================================
void SideChainAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SideChainAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

float SideChainAudioProcessor::getRmsValue(const int channel) const
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0)
        return rmsLevelLeft.getCurrentValue();
    if (channel == 1)
        return rmsLevelRight.getCurrentValue();

    return 0.f;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new SideChainAudioProcessor();
}
