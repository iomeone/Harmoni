/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/





#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
HarmoniAudioProcessorEditor::HarmoniAudioProcessorEditor (HarmoniAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (800, 800);
    addAndMakeVisible(rollComponent);
    rollComponent.setup(10, 900, 20); //default 10 bars, with 900 pixels per bar (width) and 20 pixels per step (each note height)
    
    rollComponent.sendChange = [](int note, int velocity) // Gets called every time a note is inserted or edited.
    {
//         You will probably want to send this information to some kind of MIDI messaging system
//        std::cout << "MIDI send: " << note << " : " << velocity << "\n";
        
        MidiMessageSequence::MidiEventHolder* noteEvent = nullptr;
        note = noteEvent->message.getNoteNumber();
        velocity = noteEvent->message.getVelocity();
        
        
    };
    
    tickTest = 0;
    startTimer(20);
    
    addAndMakeVisible(rollComponent);
}

HarmoniAudioProcessorEditor::~HarmoniAudioProcessorEditor()
{
}

//==============================================================================
void HarmoniAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//
//    g.setColour (juce::Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void HarmoniAudioProcessorEditor::resized()
{
    
    rollComponent.setBounds(getLocalBounds());
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}


void HarmoniAudioProcessorEditor::timerCallback()
{
    tickTest += 20;
    rollComponent.setPlaybackMarkerPosition(tickTest);
    if (tickTest >= 480 * 4 * 10) {
        tickTest = 0;
    }
}


//
//void renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
//        {
//            //make new buffer for each oscillator
//            juce::AudioBuffer<float> osc1_buf(outputBuffer.getNumChannels(), numSamples);
//
//
//            //clear new buffers
//            osc1_buf.clear();
//
//
//
//            //generate oscillator 1
//            juce::dsp::AudioBlock<float> block1(osc1_buf);
//            juce::dsp::ProcessContextReplacing<float> context1 (block1);
//            osc1.process(context1);
//
//            //add sample for each channel
//            for (auto chan = outputBuffer.getNumChannels(); --chan >= 0;){
//
//                //add to output buffer
//                outputBuffer.addFrom(chan, startSample, combined, chan, 0, numSamples);
//            }
//
//
//        }

