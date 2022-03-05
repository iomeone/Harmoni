/*
  ==============================================================================

    NavbarComponent.h
    Created: 2 Feb 2022 1:40:22pm
    Modified from Samuel Hunt: https://github.com/Sjhunt93/Piano-Roll-Editor

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
/*
 Timelime component assumes 4/4
 todo: add support for other time signatures
 */


class NavbarComponent  : public juce::Component
{
public:
    NavbarComponent();
    //~NavbarComponent() override; vet ikke hvorfor man han har ikke deconstructor

    void setup (const int barsToDraw, const int pixelsPerBar);
    void paint (juce::Graphics&); // han hadde ikke: override;
    void resized(); // han hadde ikke override;

private:
    int barsToDraw;
    int pixelsPerBar;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NavbarComponent)
};
