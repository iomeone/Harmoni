/*
  ==============================================================================

    BlockGridComponent.h
    Created: 2 Feb 2022 1:40:01pm
    Modified from Samuel Hunt: https://github.com/Sjhunt93/Piano-Roll-Editor

  ==============================================================================
*/

#pragma once

//#include <JuceHeader.h> he didn't have this here

#include "KeyNoteComponent.h"
#include "BlockGridStyleSheet.h"
//#include "MIDIExportDrag.h"

//==============================================================================
/*
*/

class ButtonBox : public juce::Component { // het Selection box
public:
    void paint (Graphics & g)
    {
        juce::Colour c = juce::Colours::darkblue;
        c = c.withAlpha((float)0.5);
        g.fillAll(c);
    }
    
    int startX, startY;
};





class BlockGridComponent  : public juce::Component, public juce::KeyListener
{
public:
    BlockGridComponent(BlockGridStyleSheet & gridStyle);
    ~BlockGridComponent(); // he didn't have override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void blockCompSelected (KeyNoteComponent *, const juce::MouseEvent&);
    void blockCompPositionMoved (KeyNoteComponent *, bool callResize = true);
    void blockCompLengthChanged (KeyNoteComponent *, int diff);
    void blockCompDragging (KeyNoteComponent*, const juce::MouseEvent&);
    void setPositions ();
    
    /* optional
    void mouseEnter (const MouseEvent&);
    void mouseExit  (const MouseEvent&);
    void mouseMove  (const MouseEvent&);
    */
    
    void mouseDown (const juce::MouseEvent&) override;
    void mouseDrag (const juce::MouseEvent&) override;
    void mouseUp (const juce::MouseEvent&) override;
    void mouseDoubleClick (const juce::MouseEvent&) override;
    
    void setupGrid (float pixelsPerBar, float compHeight, const int bars);
    void setQuantisation (const int val);
    
    bool keyPressed (const juce::KeyPress& key, Component* originatingComponent) override; // I think this is the presequal instead of originating
    void deleteAllSelected();
    
    // From here you could convert this into MIDI or any other custom musical encoding.
    PRESequence getSequence ();
    void loadSequence (PRESequence sq);
    
    float getNoteCompHeight ();
    float getPixelsPerBar ();
    
    std::vector<NoteMessage *> getSelectedMessages ();
    
    std::function<void(int note, int velocity)> sendChange;
    std::function<void()> onEdit;

private:
    void sendEdit ();
    
    BlockGridStyleSheet & gridStyle;
    ButtonBox buttonBox;
    std::vector<KeyNoteComponent *> noteComps;
    
//    MIDIExportDrag & MIDIDragButton;
    
    Array<int> blackKeys;

    
    float noteCompHeight;
    float pixelsPerBar;
    st_int ticksPerTimeSignature;
    st_int currentQValue;
    st_int lastNoteLength;
    bool firstDrag;
    bool firstCall;
    int lastTrigger;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BlockGridComponent)
};
