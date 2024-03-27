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
    setSize (565, 60);

    addAndMakeVisible(addButton);
    addButton.addListener(this);
    addAndMakeVisible(saveButton);
    saveButton.addListener(this);

    refreshMap();
}

MIDIDeckAudioProcessorEditor::~MIDIDeckAudioProcessorEditor()
{
}
 
//==============================================================================
void MIDIDeckAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MIDIDeckAudioProcessorEditor::resized()
{
    const int lineHeight = 30;
    const int border = 15;
    const int buttonWidth = 80;

    addButton.setBounds(getWidth() / 2 - buttonWidth - border, getHeight() - lineHeight - border, buttonWidth, lineHeight);
    saveButton.setBounds(getWidth() / 2 + border, getHeight() - lineHeight - border, buttonWidth, lineHeight);
}

void MIDIDeckAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &addButton)
    {
        addMap();
        return;
	}

    if (button == &saveButton)
    {
        savePreset();
        return;
    }

    // Iterate delButtonsArr and delete the corresponding indexes in the two OwnedArrays
    for (auto i = 0; i < delButtonsArr.size(); ++i)
    {
        if (button == delButtonsArr[i])
        {
            audioProcessor.midi2Cmd.erase(singleMapArr[i]->getMidiNote());
            setSize(getWidth(), getHeight() - 45);
            refreshMap();
        }
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
    // Clear all of the components in singleMapArr
    for (size_t i = 0; i < singleMapArr.size(); ++i)
    {
		removeChildComponent(singleMapArr[i]);
        removeChildComponent(delButtonsArr[i]);
	}
    
    // clear the singleMapArr
    singleMapArr.clear();
    delButtonsArr.clear();

    setSize(565, 60);

    // refresh the layout from midi2Cmd
    for (auto it = audioProcessor.midi2Cmd.begin(); it != audioProcessor.midi2Cmd.end(); ++it)
    {
        setSize(getWidth(), getHeight() + 45);

		auto newMap = std::make_unique<SingleMap>(audioProcessor);
		newMap->setMidiNote(it->first);
		newMap->setCmdPath(it->second);
        newMap->initComponent();
		newMap->setBounds(15, 15 + singleMapArr.size()* 45, 470, 30);

		singleMapArr.add(newMap.release());
		addAndMakeVisible(singleMapArr.getLast());

        auto delButton = std::make_unique<juce::TextButton>("Del");
        delButton->addListener(this);
        delButton->setBounds(500, 15 + delButtonsArr.size() * 45, 50, 30);
        delButtonsArr.add(delButton.release());
        addAndMakeVisible(delButtonsArr.getLast());
	}

    if (singleMapArr.size() != delButtonsArr.size())
        jassertfalse;
}

void MIDIDeckAudioProcessorEditor::savePreset()
{
    // Create a juce::var array
    juce::Array<juce::var> array;

    // Iterate over the map
    for (const auto& pair : audioProcessor.midi2Cmd)
    {
        // Create a juce::var object for each entry
        juce::DynamicObject* object = new juce::DynamicObject();
        object->setProperty("key", pair.first);
        object->setProperty("value", pair.second);

        // Add the object to the array
        array.add(object);
    }

    // Create a juce::var object that contains the array
    juce::DynamicObject* root = new juce::DynamicObject();
    root->setProperty("maps", array);

    // Convert the root object to JSON
    juce::String jsonString = juce::JSON::toString(root);

    // Create the file
    juce::File presetFile = juce::File::getCurrentWorkingDirectory().getChildFile("MIDIDeckPreset.json");
    if (!presetFile.exists())
    {
        presetFile.create();
    }
    // Write the JSON string to the file
    presetFile.replaceWithText(jsonString);

    // Create a window to show save success and the path
    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::NoIcon, "Save Success", "Preset saved to: " + presetFile.getFullPathName());
}