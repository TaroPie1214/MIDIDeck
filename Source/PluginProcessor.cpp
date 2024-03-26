/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MIDIDeckAudioProcessor::MIDIDeckAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

MIDIDeckAudioProcessor::~MIDIDeckAudioProcessor()
{
}

//==============================================================================
const juce::String MIDIDeckAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MIDIDeckAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MIDIDeckAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MIDIDeckAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MIDIDeckAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MIDIDeckAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MIDIDeckAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MIDIDeckAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MIDIDeckAudioProcessor::getProgramName (int index)
{
    return {};
}

void MIDIDeckAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MIDIDeckAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MIDIDeckAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MIDIDeckAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MIDIDeckAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Iterate over listened midi events and print them out
    juce::MidiBuffer::Iterator it(midiMessages);
    juce::MidiMessage message;
    int sampleNumber;

    // Reset currentMidiNote
    currentMidiNote = 128;

    while (it.getNextEvent(message, sampleNumber))
        // To avoid repeated detection(NoteOn + NoteOff), only detect NoteOn event
        if (message.isNoteOn())
            currentMidiNote = message.getNoteNumber();

    if (isAddListening)
    {
        //DBG("Listening...");
        auto it = midi2Cmd.find(currentMidiNote);
        if (currentMidiNote != 128)
        {
            if (it != midi2Cmd.end())
            {
                DBG("Note " + juce::String(currentMidiNote) + " is already assigned to command " + it->second);
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "Warning", "Note " + juce::String(currentMidiNote) + " is already assigned to command " + it->second);
            }
            else
            {
                midi2Cmd[currentMidiNote] = midi2Cmd[128] != "" ? midi2Cmd[128] : "";
                midi2Cmd.erase(128);
                isAddListening = false;
            }
        }
    }
    else
    {
        // Iterate over midi2Cmd and execute the commands
        for (auto it = midi2Cmd.begin(); it != midi2Cmd.end(); ++it)
        {
            if (it->first == currentMidiNote && it->first != 128)
            {
                // Excute the command
                if (it->second != "")
                {
                    CommandThread commandThread(it->second);
                    commandThread.startThread();
                }
            }
        }
    }
 }

//==============================================================================
bool MIDIDeckAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MIDIDeckAudioProcessor::createEditor()
{
    return new MIDIDeckAudioProcessorEditor (*this);
}

//==============================================================================
void MIDIDeckAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MIDIDeckAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MIDIDeckAudioProcessor();
}
