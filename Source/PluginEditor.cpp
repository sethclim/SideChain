/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Backend/PresetManager/PresetManager.h"
#include "UI/PresetManagerUI.h"
#include "DraggableNodeIdentifiers.h"

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
      presetPanel(p.getPresetManager(), p.GetAPVTS())
{
  setLookAndFeel(&otherLookAndFeel);
  setResizable(true, true);
  setResizeLimits(500, 300, 1000, 600);
  setSize(500, 300);

  addAndMakeVisible(&volLabel, -1);
  addAndMakeVisible(&relLabel, -1);
  addAndMakeVisible(&DynamicCurveEditor);

  addAndMakeVisible(&verticalMeterL);
  addAndMakeVisible(&verticalMeterR);

  addAndMakeVisible(divisionMenu);
  // divisionMenu.addItem("Eighth", Eighth);
  // divisionMenu.addItem("Quarter", Quarter);
  // divisionMenu.addItem("Half", Half);
  // divisionMenu.addItem("Whole", Whole);

  DBG(p.GetAPVTS().state.toXmlString());

  auto *parameter = p.GetAPVTS().getParameter("divisions");
  divisionMenu.addItemList(parameter->getAllValueStrings(), 1);

  divisionMenu.setSelectedId(2);

  divisionChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.GetAPVTS(), "divisions", divisionMenu);

  divisionParamAttachment = std::make_unique<juce::ParameterAttachment>(
      *parameter,
      [this, parameter](float denormalisedValue)
      { visageView.setDivisionLabel(parameter->getText(parameter->convertTo0to1(denormalisedValue), 0).toStdString()); },
      nullptr);
  divisionParamAttachment->sendInitialUpdate();

  visageView.setDivisionClickedCallback([this, parameter]()
                                        {
      auto currentIndex = (int) std::round(parameter->convertFrom0to1(parameter->getValue()));
      auto nextIndex = (currentIndex + 1) % parameter->getNumSteps();
      divisionParamAttachment->setValueAsCompleteGesture((float) nextIndex); });

  p.getCurveManager().registerOnCalculateDataPointsCallback([this](std::vector<juce::Point<float>> points)
                                                            {
    std::vector<std::pair<float, float>> normalized;
    normalized.reserve((size_t) points.size());
    for (auto &point : points)
      normalized.emplace_back(point.x, 1.0f - point.y);
    visageView.setCurvePoints(std::move(normalized)); });

  visageView.setOnCurveNodeDragged([this](int index, float x, float y)
                                   {
    auto base = audioProcessor.GetAPVTS().state.getChildWithName(DraggableNodeIdentifiers::myRootDraggableTreeType);
    auto child = base.getChild(index);
    if (!child.isValid())
      return;

    auto id = child.getProperty(DraggableNodeIdentifiers::id);
    audioProcessor.getCurveManager().moveNode(id, juce::Point<float>(x, y)); });

  addAndMakeVisible(&presetPanel);
}

SideChainAudioProcessorEditor::~SideChainAudioProcessorEditor()
{
  visageView.remove();
  setLookAndFeel(nullptr);
}

void SideChainAudioProcessorEditor::parentHierarchyChanged()
{
  if (visageEmbedded)
    return;

  if (auto *peer = getPeer())
  {
    visageView.embed(peer->getNativeHandle(), getWidth(), getHeight());
    visageEmbedded = true;
  }
}

//==============================================================================
void SideChainAudioProcessorEditor::paint(juce::Graphics &g)
{
  // (Our component is opaque, so we must completely fill the background with a solid colour)
  g.fillAll(juce::Colours::black);
  g.setColour(juce::Colours::white);
  g.setFont(15.0f);
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
      juce::GridItem(divisionMenu).withArea(5, 1, 5, 2),
      juce::GridItem(volLabel).withArea(5, 2, 5, 3),
      juce::GridItem(verticalMeterL).withArea(2, 4, 5, 4),
      juce::GridItem(verticalMeterR).withArea(2, 5, 5, 5),
  });

  grid.performLayout(getLocalBounds());

  if (visageEmbedded)
    visageView.resize(getWidth(), getHeight());
}
