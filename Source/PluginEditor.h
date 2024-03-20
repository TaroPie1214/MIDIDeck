/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/SingleMap.h"

//==============================================================================
/**
*/
class MIDIDeckAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener
{
public:
    MIDIDeckAudioProcessorEditor (MIDIDeckAudioProcessor&);
    ~MIDIDeckAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;

    void addMap();
    void removeMap(int index);

private:
    MIDIDeckAudioProcessor& audioProcessor;

    juce::OwnedArray<SingleMap> dynamicMaps;
    int mapCount = 0;

    // Add button
    juce::TextButton addButton{ "Add..." };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MIDIDeckAudioProcessorEditor)
};
