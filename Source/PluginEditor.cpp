/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MIDIDeckAudioProcessorEditor::MIDIDeckAudioProcessorEditor (MIDIDeckAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (500, 60);

    addAndMakeVisible(addButton);
    addButton.addListener(this);
}

MIDIDeckAudioProcessorEditor::~MIDIDeckAudioProcessorEditor()
{
}

//==============================================================================
void MIDIDeckAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void MIDIDeckAudioProcessorEditor::resized()
{
    const int lineHeight = 30;
    const int border = 15;
    const int buttonWidth = 80;

    addButton.setBounds(getWidth() / 2 - buttonWidth / 2, getHeight() - lineHeight - border, buttonWidth, lineHeight);
}

void MIDIDeckAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &addButton)
    {
        addMap();
	}
}

void MIDIDeckAudioProcessorEditor::addMap()
{
    // Use 128 as a flag to indicate whether there is a new default mapping
    auto it = audioProcessor.midi2Cmd.find(128);
    if (it != audioProcessor.midi2Cmd.end())
    {
        DBG("Default mapping already exists");
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "Warning", "Default mapping already exists");
    }
    else
    {
        audioProcessor.midi2Cmd[128] = "";
        setSize(getWidth(), getHeight() + 45);
        refreshMap();
    }
}

void MIDIDeckAudioProcessorEditor::removeMap(int key)
{
    audioProcessor.midi2Cmd.erase(key);
	refreshMap();
}

void MIDIDeckAudioProcessorEditor::refreshMap()
{
    // clear all of the components in dynamicMaps
    for (size_t i = 0; i < dynamicMaps.size(); ++i)
    {
		removeChildComponent(dynamicMaps[i]);
	}
    
    // clear the dynamicMaps
    dynamicMaps.clear();

    // Sort the contents of the dictionary according to key value
    

    // refresh the layout from midi2Cmd
    for (auto it = audioProcessor.midi2Cmd.begin(); it != audioProcessor.midi2Cmd.end(); ++it)
    {
		auto newMap = std::make_unique<SingleMap>(audioProcessor);
		newMap->setMidiNote(it->first);
		newMap->setCmdPath(it->second);
        newMap->initComponent();
		newMap->setBounds(15, 15 + dynamicMaps.size()* 45, 470, 30);

		dynamicMaps.add(newMap.release());
		addAndMakeVisible(dynamicMaps.getLast());
	}
}