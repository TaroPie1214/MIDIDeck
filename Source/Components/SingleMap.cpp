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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

SingleMap::~SingleMap()
{
}

void SingleMap::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);   // clear the background
}

void SingleMap::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
