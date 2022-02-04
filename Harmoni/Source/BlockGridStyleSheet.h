/*
  ==============================================================================

    BlockGridStyleSheet.h
    Created: 2 Feb 2022 2:36:21pm
    Author:  Axel Borgmo

  ==============================================================================
*/

#pragma once

//#include <JuceHeader.h> han hadde ikke dette
#include "PConstants.h"


/*
 TODO extend this class so that it controls other elements of style, i.e. colours..
 */


//==============================================================================
/*
*/
class BlockGridStyleSheet
{
public:
    /*
     We use a friend class to ensure a one-way only relationship. The control panel can set member varialbes easily. Saves adding loads of getters/setters
     */
    friend class BlockGridControlPanel;
    
    
    BlockGridStyleSheet();
    // han hadde ikke deconstructor: ~BlockGridStyleSheet() override;

    bool getDrawMIDINum ();
    bool getDrawMIDINoteStr ();
    bool getDrawVelocity ();
    
    bool disableEditing;
    
private:
    
    bool drawMIDINum, drawMIDINoteStr, drawVelocity;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BlockGridStyleSheet)
};
