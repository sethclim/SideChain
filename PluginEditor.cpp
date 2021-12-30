/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SideChainAudioProcessorEditor::SideChainAudioProcessorEditor (SideChainAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), DynamicCurveEditor(p.dynamicCurve, p.dynamicCurve.draggableNodes)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    
    setResizable(true, true);
    setResizeLimits(400, 360, 1000, 600);
    setSize (400, 300);
    
    addAndMakeVisible(&DynamicCurveEditor);
}

SideChainAudioProcessorEditor::~SideChainAudioProcessorEditor()
{
}

//==============================================================================
void SideChainAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World! SideChain", getLocalBounds(), juce::Justification::centred, 1);
}

void SideChainAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    juce::Grid grid;
    
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
       
    grid.templateRows    = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };
    grid.templateColumns = { Track (Fr (2)), Track (Fr (4)), Track (Fr (1)) };

    grid.items.addArray({
        juce::GridItem (DynamicCurveEditor).withArea(1, 1, 3, 3),
        });
    


    grid.performLayout (getLocalBounds());
    
}
