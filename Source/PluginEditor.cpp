/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SideChainAudioProcessorEditor::SideChainAudioProcessorEditor(SideChainAudioProcessor &p)
    : AudioProcessorEditor(&p),
      audioProcessor(p),
      DynamicCurveEditor(p.curveManager),
      volLabel(p.envelopeProcessor.currentVol),
      relLabel(p.envelopeProcessor.relPosition),
      modeText("Off")
{
  setResizable(true, true);
  setResizeLimits(500, 300, 1000, 600);
  setSize(500, 300);

  addAndMakeVisible(&volLabel, -1);
  addAndMakeVisible(&relLabel, -1);
  addAndMakeVisible(&DynamicCurveEditor);

  addAndMakeVisible(&modeText, -1);

  modeText.onClick = [this]
  { OnModeTextClicked(); };

  addAndMakeVisible(divisionMenu);
  divisionMenu.addItem("Eighth", Eighth);
  divisionMenu.addItem("Quarter", Quarter);
  divisionMenu.addItem("Half", Half);
  divisionMenu.addItem("Whole", Whole);

  divisionMenu.onChange = [this]
  { divisionMenuChanged(); };
  divisionMenu.setSelectedId(2);
}

void SideChainAudioProcessorEditor::OnModeTextClicked()
{
  DynamicCurveEditor.SetDragAreaMode(!DynamicCurveEditor.GetDragAreaMode());
  repaint();
}

void SideChainAudioProcessorEditor::divisionMenuChanged()
{

  switch (divisionMenu.getSelectedId())
  {
  case 1:
    audioProcessor.envelopeProcessor.SetDivisions(2);
    break;
  case 2:
    audioProcessor.envelopeProcessor.SetDivisions(1);
    break;
  case 3:
    audioProcessor.envelopeProcessor.SetDivisions(0.5);
    break;
  case 4:
    audioProcessor.envelopeProcessor.SetDivisions(0.25);
    break;
  default:
    break;
  }
}

SideChainAudioProcessorEditor::~SideChainAudioProcessorEditor() = default;

//==============================================================================
void SideChainAudioProcessorEditor::paint(juce::Graphics &g)
{
  // (Our component is opaque, so we must completely fill the background with a solid colour)
  g.fillAll(juce::Colours::black);

  g.setColour(juce::Colours::white);
  g.setFont(15.0f);
  modeText.setButtonText(DynamicCurveEditor.GetDragAreaMode() ? "ON" : "OFF");
}

void SideChainAudioProcessorEditor::resized()
{
  juce::Grid grid;

  using Track = juce::Grid::TrackInfo;
  using Fr = juce::Grid::Fr;

  grid.templateRows = {Track(Fr(2)), Track(Fr(2)), Track(Fr(1)), Track(Fr(1))};
  grid.templateColumns = {Track(Fr(3)), Track(Fr(3)), Track(Fr(3)), Track(Fr(1))};

  grid.items.addArray({
      juce::GridItem(DynamicCurveEditor).withArea(1, 1, 3, 4),
      juce::GridItem(modeText).withArea(3, 1, 4, 2),
      juce::GridItem(divisionMenu).withArea(3, 2, 4, 3),
      juce::GridItem(relLabel).withArea(3, 3, 4, 4),
  });

  // auto cM = audioProcessor.curveManager;
  // DynamicCurveEditor.setBounds(0, 0, cM.width, cM.height);
  grid.performLayout(getLocalBounds());
}
