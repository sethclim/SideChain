/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SideChainAudioProcessorEditor::SideChainAudioProcessorEditor (SideChainAudioProcessor& p)
    : AudioProcessorEditor (&p),
      audioProcessor (p),
      DynamicCurveEditor(p.curveManager),
      volLabel(p.envelopeProcessor.currentVol),
      relLabel(p.envelopeProcessor.relPosition)
{
    setResizable(true, true);
    setResizeLimits(audioProcessor.curveManager.width, 360, 1000, 600);
    setSize (audioProcessor.curveManager.width, 360);
    
    addAndMakeVisible(&volLabel, -1);
    addAndMakeVisible(&relLabel, -1);
    addAndMakeVisible(&DynamicCurveEditor);
}

SideChainAudioProcessorEditor::~SideChainAudioProcessorEditor() = default;

//==============================================================================
void SideChainAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
}

void SideChainAudioProcessorEditor::resized()
{
    juce::Grid grid;
    
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
       
    grid.templateRows    = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };
    grid.templateColumns = { Track (Fr (2)), Track (Fr (4)), Track (Fr (1)) };

    grid.items.addArray({
        juce::GridItem (volLabel).withArea(3, 1, 4, 3),
        juce::GridItem (relLabel).withArea(3, 2, 4, 3),
    });

    auto cM = audioProcessor.curveManager;
    DynamicCurveEditor.setBounds(0,0,cM.width, cM.height);

    grid.performLayout (getLocalBounds());
}
