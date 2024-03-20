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

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
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
        setSize(getWidth(), getHeight() + 45);
        addMap();
	}
}

void MIDIDeckAudioProcessorEditor::addMap()
{
    auto newMap = std::make_unique<SingleMap>();

    newMap->setBounds(15, 15 + mapCount * 45, 470, 30);

    // 将新组件添加到动态组件数组中
    dynamicMaps.add(newMap.release());

    // 将新组件添加到PluginEditor并使其可见
    addAndMakeVisible(dynamicMaps.getLast());

    mapCount++;
}

void MIDIDeckAudioProcessorEditor::removeMap(int index)
{
	// 从动态组件数组中删除组件
	dynamicMaps.remove(index);

	// 从PluginEditor中删除组件
	removeChildComponent(dynamicMaps[index]);

	mapCount--;
}