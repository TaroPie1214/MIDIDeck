/*
  ==============================================================================

    CommandThread.h
    Created: 26 Mar 2024 11:19:12am
    Author:  sunwei06

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
        // 在析构函数中停止线程
        stopThread(10000);
    }

    void run() override
    {
        juce::ChildProcess childProcess;

        if (childProcess.start(command))
        {
            // 等待进程完成
            childProcess.waitForProcessToFinish(10000);

            // 获取输出
            juce::String output = childProcess.readAllProcessOutput();

            // 打印输出
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