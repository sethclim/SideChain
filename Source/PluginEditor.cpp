/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Backend/PresetManager/PresetManager.h"
#include "UI/PresetManagerUI.h"

//==============================================================================
SideChainAudioProcessorEditor::SideChainAudioProcessorEditor(SideChainAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p),
      verticalMeterL([&]()
                     { return audioProcessor.getRmsValue(0); }),
      verticalMeterR([&]()
                     { return audioProcessor.getRmsValue(1); }),
      DynamicCurveEditor(p.getCurveManager(), p.GetAPVTS()),
      volLabel(p.envelopeProcessor.currentVol),
      relLabel(p.envelopeProcessor.relPosition),
      modeText("Off"),
      presetPanel(p.getPresetManager(), p.GetAPVTS())
{
  setLookAndFeel(&otherLookAndFeel);
  setResizable(true, true);
  setResizeLimits(500, 300, 1000, 600);
  setSize(500, 300);

  addAndMakeVisible(&volLabel, -1);
  addAndMakeVisible(&relLabel, -1);
  addAndMakeVisible(&DynamicCurveEditor);

  addAndMakeVisible(&modeText, -1);
  addAndMakeVisible(&verticalMeterL);
  addAndMakeVisible(&verticalMeterR);

  modeText.onClick = [this]
  { OnModeTextClicked(); };

  addAndMakeVisible(divisionMenu);
  // divisionMenu.addItem("Eighth", Eighth);
  // divisionMenu.addItem("Quarter", Quarter);
  // divisionMenu.addItem("Half", Half);
  // divisionMenu.addItem("Whole", Whole);

  DBG(p.GetAPVTS().state.toXmlString());

  auto *parameter = p.GetAPVTS().getParameter("divisions");
  divisionMenu.addItemList(parameter->getAllValueStrings(), 1);

  //divisionMenu.onChange = [this]
  //{ divisionMenuChanged(); };
  divisionMenu.setSelectedId(2);

  divisionChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.GetAPVTS(), "divisions", divisionMenu);

  addAndMakeVisible(&presetPanel);
}

SideChainAudioProcessorEditor::~SideChainAudioProcessorEditor()
{
  setLookAndFeel(nullptr);
}

void SideChainAudioProcessorEditor::OnModeTextClicked()
{
  DynamicCurveEditor.SetDragAreaMode(!DynamicCurveEditor.GetDragAreaMode());  
  repaint();
}

//void SideChainAudioProcessorEditor::divisionMenuChanged()
//{
// /* switch (divisionMenu.getSelectedId())
//  {
//  case 1:
//    audioProcessor.envelopeProcessor.SetDivisions(2);
//    break;
//  case 2:
//    audioProcessor.envelopeProcessor.SetDivisions(1);
//    break;
//  case 3:
//    audioProcessor.envelopeProcessor.SetDivisions(0.5);
//    break;
//  case 4:
//    audioProcessor.envelopeProcessor.SetDivisions(0.25);
//    break;
//  default:
//    break;
//  }*/
//}

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

  grid.templateRows = {Track(Fr(1)), Track(Fr(2)), Track(Fr(2)), Track(Fr(1)), Track(Fr(1))};
  grid.templateColumns = {Track(Fr(3)), Track(Fr(3)), Track(Fr(3)), Track(Fr(1)), Track(Fr(1))};

  grid.items.addArray({
      juce::GridItem(presetPanel).withArea(1, 1, 1, 5),
      juce::GridItem(DynamicCurveEditor).withArea(2, 1, 5, 4),
      juce::GridItem(modeText).withArea(5, 1, 5, 2),
      juce::GridItem(divisionMenu).withArea(5, 2, 5, 3),
      juce::GridItem(volLabel).withArea(5, 3, 5, 4),
      juce::GridItem(verticalMeterL).withArea(2, 4, 5, 4),
      juce::GridItem(verticalMeterR).withArea(2, 5, 5, 5),
  });

  grid.performLayout(getLocalBounds());
}
