#pragma once

#include <JuceHeader.h>

namespace Service
{
    class PresetManager : ValueTree::Listener
    {
    public:
        static const File defaultDirectory;
        static const String extension;
        static const String presetNameProperty;

        PresetManager(juce::AudioProcessorValueTreeState& apvts);

        void savePreset(const String &presetName);
        void deletePreset(const String &presetName);
        void loadPreset(const String &presetName);
        int loadNextPreset();
        int loadPreviousPreset();
        StringArray getAllPresets() const;
        String getCurrentPreset() const;

    private:
        void valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged) override;

        juce::AudioProcessorValueTreeState& valueTreeState;
        Value currentPreset;
    };
}
