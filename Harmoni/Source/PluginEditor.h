/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PianoRollComponent.h"


// Tror jeg må gjøre om denne til sånn som MainComponent ser ut?


//==============================================================================
/**
*/
class HarmoniAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    HarmoniAudioProcessorEditor (HarmoniAudioProcessor&);
    ~HarmoniAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HarmoniAudioProcessor& audioProcessor;
    
    PianoRollComponent rollComponent; // object originally called editor
    
    st_int tickTest; //Need to use PConstans first I believe

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmoniAudioProcessorEditor)
};
