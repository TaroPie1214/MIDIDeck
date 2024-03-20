/*
  ==============================================================================

    SingleMap.cpp
    Created: 19 Mar 2024 10:56:44am
    Author:  TaroPie

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SingleMap.h"

//==============================================================================
SingleMap::SingleMap()
{
    fileComp.reset(new juce::FilenameComponent("fileComp",
        {},                       // current file
        false,                    // can edit file name,
        false,                    // is directory,
        false,                    // is for saving,
        {},                       // browser wildcard suffix,
        {},                       // enforced suffix,
        "Select file to open"));  // text when nothing selected
    addAndMakeVisible(fileComp.get());
    fileComp->addListener(this);

    startListeningButton.addListener(this);
    addAndMakeVisible(startListeningButton);
}

SingleMap::~SingleMap()
{
}

void SingleMap::paint (juce::Graphics& g)
{
    //g.fillAll(juce::Colours::black);
}

void SingleMap::resized()
{
    startListeningButton.setBounds(0, 0, 150, 30);
    fileComp->setBounds(165, 0, getWidth() - 165, 30);
}

void SingleMap::filenameComponentChanged(juce::FilenameComponent* fileComponentThatHasChanged)
{
    //if (fileComponentThatHasChanged == fileComp.get())
    //    readFile(fileComp->getCurrentFile());

    juce::String storeLocationTemp = fileComp->getCurrentFileText();
}

void SingleMap::readFile(const juce::File& fileToRead)
{
    if (!fileToRead.existsAsFile())
        return;

    auto fileText = fileToRead.loadFileAsString();
}

void SingleMap::buttonClicked(juce::Button* button)
{
    if (button == &startListeningButton)
    {
		DBG("Start Listening");
	}
}