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
    setSize (400, 60);

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
    const int borderHeight = 15;
    const int buttonWidth = 80;

    addButton.setBounds(getWidth() / 2 - buttonWidth / 2, getHeight() - lineHeight - borderHeight, buttonWidth, lineHeight);
}

void MIDIDeckAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &addButton)
    {
        setSize(getWidth(), getHeight() + 60);
        addMap();
	}
}

void MIDIDeckAudioProcessorEditor::addMap()
{
    auto newMap = std::make_unique<SingleMap>();

    newMap->setBounds(10, 10 + mapCount * 50, 50, 50);

    // ���������ӵ���̬���������
    dynamicMaps.add(newMap.release());

    // ���������ӵ�PluginEditor��ʹ��ɼ�
    addAndMakeVisible(dynamicMaps.getLast());

    mapCount++;
}

void MIDIDeckAudioProcessorEditor::removeMap(int index)
{
	// �Ӷ�̬���������ɾ�����
	dynamicMaps.remove(index);

	// ��PluginEditor��ɾ�����
	removeChildComponent(dynamicMaps[index]);

	mapCount--;
}