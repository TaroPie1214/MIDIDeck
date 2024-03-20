/*
  ==============================================================================

    SingleMap.h
    Created: 19 Mar 2024 10:56:44am
    Author:  TaroPie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SingleMap  : public juce::Component, public juce::FilenameComponentListener, public juce::Button::Listener
{
public:
    SingleMap();
    ~SingleMap() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void filenameComponentChanged(juce::FilenameComponent* fileComponentThatHasChanged) override;
    void readFile(const juce::File& fileToRead);

    void buttonClicked(juce::Button* button) override;

private:
    int midiNote = 0;
    juce::String cmdPath = "";

    std::unique_ptr<juce::FilenameComponent> fileComp;
    juce::TextButton startListeningButton{ "Start Listening" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SingleMap)
};
