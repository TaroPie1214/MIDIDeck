/*
  ==============================================================================

    SingleMap.h
    Created: 19 Mar 2024 10:56:44am
    Author:  TaroPie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

//==============================================================================
/*
*/
class SingleMap  : public juce::Component, public juce::FilenameComponentListener, public juce::Button::Listener, public juce::Timer
{
public:
    SingleMap(MIDIDeckAudioProcessor&);
    ~SingleMap() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void filenameComponentChanged(juce::FilenameComponent* fileComponentThatHasChanged) override;
    void readFile(const juce::File& fileToRead);

    void buttonClicked(juce::Button* button) override;

    int getMidiNote() { return midiNote; }
    void setMidiNote(int note) { midiNote = note; }
    void setCmdPath(juce::String path) { cmdPath = path; }


    void initComponent();

    void timerCallback() override;

private:
    MIDIDeckAudioProcessor& audioProcessor;

    int midiNote = 0;
    int index = 0;
    juce::String cmdPath = "";

    std::unique_ptr<juce::FilenameComponent> fileComp;
    juce::TextButton startListeningButton{ "Start Listening" };
    juce::Label midiNoteLabel;
    
    int pointCounter = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SingleMap)
};
