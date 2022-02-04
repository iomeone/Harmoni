/*
  ==============================================================================

    BlockGridControlPanel.h
    Created: 2 Feb 2022 1:42:10pm
    Author:  Axel Borgmo

  ==============================================================================
*/

#pragma once

// Not sure if needed #include <JuceHeader.h>
#include "PConstants.h"
#include "BlockGridComponent.h"
#include "BlockGridStyleSheet.h"

//==============================================================================
/*
*/
class BlockGridControlPanel  : public juce::Component
{
public:
    BlockGridControlPanel(BlockGridComponent & component, BlockGridStyleSheet & gridStyle);
    ~BlockGridControlPanel(); // hadde ikke her: override;

    void resized();// hadde ikke her: override;
    void paint (juce::Graphics&);//hadde ikke her: override;
    
    void renderSequence();
    std::function<void(int pixelsPerBar, int noteHeight)> configureGrid;
    
    void setQuantisation (PRE::eQuantisationValue value);
    

private:
    
    BlockGridComponent & blockGrid;
    BlockGridStyleSheet & gridStyle;
    
    Slider noteCompHeight, pixelsPerBar;
    
    TextButton render;
    ToggleButton drawMIDINotes, drawMIDIText, drawVelocity;
    
    ComboBox quantisationValue;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BlockGridControlPanel)
};
