/*
  ==============================================================================

    BlockGridStyleSheet.cpp
    Created: 2 Feb 2022 2:36:21pm
    Author:  Axel Borgmo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "BlockGridStyleSheet.h"

//==============================================================================
BlockGridStyleSheet::BlockGridStyleSheet()
{
    drawMIDINum = false;
    drawMIDINoteStr = false;
    drawVelocity = true;
    disableEditing = false;

}

//BlockGridStyleSheet::~BlockGridStyleSheet()
//{
//}


bool BlockGridStyleSheet::getDrawMIDINum ()
{
    return drawMIDINum;
}
bool BlockGridStyleSheet::getDrawMIDINoteStr ()
{
    return drawMIDINoteStr;
}
bool BlockGridStyleSheet::getDrawVelocity ()
{
    return drawVelocity;
}



