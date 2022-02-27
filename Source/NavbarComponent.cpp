/*
  ==============================================================================

    NavbarComponent.cpp
    Created: 2 Feb 2022 1:40:22pm
    Author:  Axel Borgmo

  ==============================================================================
*/

//#include <JuceHeader.h>
#include "NavbarComponent.h"

//==============================================================================
NavbarComponent::NavbarComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    barsToDraw = 0;
    pixelsPerBar = 0;

}

//NavbarComponent::~NavbarComponent()
//{
//}


void NavbarComponent::setup (const int bd, const int px)
{
    barsToDraw = bd;
    pixelsPerBar = px;
    setSize(pixelsPerBar * barsToDraw, getHeight()); //height is set externally.
}





void NavbarComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(12, 20, 31));

    // Looks like you haven't called setup??
    if (pixelsPerBar == 0 || barsToDraw == 0) {
        jassertfalse;
        return;
    }
    
    const int marks = barsToDraw * 4; //assume 4/4
    const float increment = getWidth() / (float)(marks);
    float yPos = 0;
    
    g.setColour(juce::Colours::white);
    
    for (int i = 0; i < marks; i++) {
        
        if (i % 4 == 0) {
            const juce::String txt(i/4 + 1);
            g.drawText(txt, yPos + 5, 3, 30, 20, juce::Justification::left);
            g.drawLine(yPos, 0, yPos, getHeight());
        }
        else if ( i % 2 == 0) {
            g.drawLine(yPos, getHeight() * 0.66, yPos, getHeight());
        }
        else {
            g.drawLine(yPos, getHeight() * 0.33, yPos, getHeight());
        }
        
        yPos += increment;
    }
}

void NavbarComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
