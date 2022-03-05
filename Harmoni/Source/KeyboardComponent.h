/*
  ==============================================================================

    KeyboardComponent.h
    Created: 2 Feb 2022 1:40:37pm
    Modified from Samuel Hunt: https://github.com/Sjhunt93/Piano-Roll-Editor

  ==============================================================================
*/

#pragma once

//#include <JuceHeader.h>

#include "PConstants.h"

//==============================================================================
/*
*/
class KeyboardComponent  : public juce::Component
{
public:
    KeyboardComponent();
    //~KeyboardComponent() override;

    void paint (juce::Graphics&) override;
    //void resized() override;

private:
    
    Array<int> blackKeys;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyboardComponent)
};
