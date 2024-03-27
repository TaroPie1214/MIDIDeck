/*
  ==============================================================================

    CommandThread.h
    Created: 26 Mar 2024 11:19:12am
    Author:  TaroPie

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CommandThread  : public juce::Thread
{
public:
    CommandThread(const juce::String& command) : juce::Thread("CommandThread"), command(command)
    {
    } 
    ~CommandThread()
    {
        stopThread(10000);
    }

    void run() override
    {
        juce::ChildProcess childProcess;

        if (childProcess.start(command))
        {
            childProcess.waitForProcessToFinish(10000);
            juce::String output = childProcess.readAllProcessOutput();
            juce::Logger::writeToLog(output);
        }
        else
        {
            juce::Logger::writeToLog("Failed to start process");
        }
    }

private:
    juce::String command;
};