/*
  ==============================================================================

    KeyboardComponent.cpp
    Created: 2 Feb 2022 1:40:37pm
    Author:  Axel Borgmo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KeyboardComponent.h"

//==============================================================================
KeyboardComponent::KeyboardComponent()
{
    blackKeys.add(1);
    blackKeys.add(3);
    blackKeys.add(6);
    blackKeys.add(8);
    blackKeys.add(10);

}

//KeyboardComponent::~KeyboardComponent() he didn't have
//{
//}

void KeyboardComponent::paint (juce::Graphics& g)
{
    const float noteCompHeight = getHeight() / 128.0;
    {
        float line = 0;//noteCompHeight;
        
        for (int i = 127; i >= 0; i--) {
            
            const int pitch = i % 12;
            g.setColour ( blackKeys.contains(pitch) ? (Colours::darkgrey) : Colours::lightgrey.darker() );
        
            g.fillRect(0, (int)line, getWidth(), (int)noteCompHeight); //explicit cast to int otherwise matching function error
            
            g.setColour(Colours::white);
            g.drawText(String(i), 5, line, 40, noteCompHeight, Justification::left);
            
            line += noteCompHeight;
            
            g.setColour(Colours::black);
            g.drawLine(0, line, getWidth(), line);
        }
    }
}

//void KeyboardComponent::resized()
//{
//    // This method is where you should set the bounds of any child
//    // components that your component contains..
//
//}
