/*
  ==============================================================================

    PianoRollComponent.h
    Created: 1 Feb 2022 9:00:03pm
    Modified from Samuel Hunt: https://github.com/Sjhunt93/Piano-Roll-Editor

  ==============================================================================
*/

#pragma once

//#include <JuceHeader.h> He didn't have


#include "PConstants.h"
#include "BlockGridControlPanel.h"
#include "NavbarComponent.h"
#include "KeyboardComponent.h"


class ScrollDrag : public juce::Viewport // A Viewport is used to contain a larger child component, and allows the child to be automatically scrolled around.
{
public:
    void visibleAreaChanged (const juce::Rectangle<int>& newVisibleArea)
    {
        Viewport::visibleAreaChanged(newVisibleArea);
        if (positionMoved) {
            positionMoved(getViewPositionX(), getViewPositionY());
        }
    }
    
    std::function<void(int,int)> positionMoved;
};

/* 
 
 
 |------|------------------------------------------
 |      |
 |      |   menu
 |      |
 |------|------------------------------------------
 |      |   navbar component
 |______|__________________________________________
 |      |
 |      |
 |keys  |   main viewport and note grid component
 |      |
 |      |
 |      |
 |------|-------------------------------------------
 |      |
 |      |   BlockGridControlPanel
 |      |
 |------|-------------------------------------------
 
 */




//==============================================================================
/*
*/
class PianoRollComponent  : public juce::Component, public MenuBarModel
{
public:
    
    struct ExternalModelEditor {  // change this name also. Not sure what it is though
        std::vector<NoteMessage *> messages; // not sure what notemodel is, skrev NoteMessage isteden, so change name later. Also models was changed to message
        
        // tror kanskje at hver gang han skriver note så kan jeg si brick!
        // his comments for the above is: //const event pointers but mutable elements
        std::function<void()> update; // update variable name
        // his comments for the above line is: //once you have made the edits then call this
        
        
    };
    
    
    PianoRollComponent();
    ~PianoRollComponent();
    
    


    
    
    /* his comments here are:
     
     This needs to be called after the constructor and determines the size of the grid.
     
     Once setup the number of bars can be dynamically altered through @updateBars(..
     
     Todo: automatically resize the number of bars
     
     
     
     
     */
    
//    menu bar
    
    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex (int index, const String& name) override;
    void menuItemSelected (int menuID, int index) override;
    
//
    
    
    
    void setup (const int bars, const int pixelsPerBar, const int blockHeight);
    void updateBars (const int newNumberOfBars);
    

    


    void paint (juce::Graphics&) override;
//    void paintOverChildren (Graphics&) override;
    void resized() override;
    
    void showControlPanel (bool state);
    void showMenu(bool state); //optional I think, thats why there's staet
    
    
    // According to him, presequence is the input and output to the grid. I think maybe I should look into the midi class instead, but lets try.
    
    
    
    
    /*
     PRE Sequence is essentially the input and output to the grid, i.e. the data model abstracted away from the GUI
     The GUI creates a PRESequence
     The GUI understands how to render a PRESequence
     
     Although this approach is probably inefficent, its unlikely to cause realtime performance issues...
     */
    void loadSequence (PRESequence sequence);
    PRESequence getSequence();
//    PRESequence ExportMIDI();
    
    void setScroll (double x, double y);
    void setPlaybackMarkerPosition (const st_int ticks, bool isVisible = true);
    
    
    /*
     If you just want to view the editor then disable the grid.
     */
    void disableEditing (bool value);
    BlockGridControlPanel & getControlPanel ();
    
    /*
     Returns any notes that are selected used in IGME
     */
    ExternalModelEditor getSelectedNoteMessages ();
    
    /*
     This is called when the grid is edited.
     */
    std::function<void()> onEdit;
    
    /*
     You can use this to implement simple MIDI synthesis when notes are being edited,
     when notes are edited this function will be called
     */
    std::function<void(int note, int velocity)> sendChange;
    
    
//    /* menu same as control panel*/
//    MenuComponent & getMenu();
    
    
    
//    menu
    enum MenuIDs {
        midiImport = 1000,
        midiExport,
        similarSongs,
        SliderMax
    };
    
        
    

private:
    
    
    
    BlockGridStyleSheet gridStyle;
    

    
    BlockGridComponent      noteGrid; //the actual piano roll
    NavbarComponent         navbarComp; // the timeline marker at the top
    KeyboardComponent       keyboardComp; // the keyboard visualiser to the left
    
    
    ScrollDrag          scrollGrid, scrollNavbar, scrollKeyboard;
    
    

    
    BlockGridControlPanel    controlPanel;
    
    st_int  playbackTicks;
    bool    showPlaybackMarker;
    
    
    
//    menu
    Label label;
    Slider slider;
    MenuBarComponent menuBar;
    ApplicationProperties appProperties;
    
    
//    midi export
    void loadMIDIFile();
    
    
//    chordDisplay
    juce::TextEditor chordDisplay;
    
    void logMessage (const juce::String& m)
    {
        chordDisplay.clear();

//        chordDisplay.onTextChange = [this]
//        {
//            chordDisplay.setSize (chordDisplay.getTextWidth(), chordDisplay.getTextHeight());
//            chordDisplay.setCentrePosition (chordDisplay.getParentWidth() / 2, chordDisplay.getParentHeight() / 2);
//        };
//        chordDisplay.setColour (juce::Font::textColourId, Colours::black);
//        chordDisplay.setEditable(false);
        chordDisplay.applyFontToAllText (juce::Font (14, juce::Font::plain));
        chordDisplay.applyColourToAllText(juce::Colours::orange);
        chordDisplay.setJustification(36);
        chordDisplay.insertTextAtCaret (m);
    }
    

    
    void addChordToDisplay ()
    {
//        auto time = message.getTimeStamp();
//
//        auto hours   = ((int) (time / 3600.0)) % 24;
//        auto minutes = ((int) (time / 60.0)) % 60;
//        auto seconds = ((int) time) % 60;
//        auto millis  = ((int) (time * 1000.0)) % 1000;
//
//        auto timecode = juce::String::formatted ("%02d:%02d:%02d.%03d",
//                                                 hours,
//                                                 minutes,
//                                                 seconds,
//                                                 millis);
//        logMessage (timecode + "  -  " + getMidiMessageDescription (message));
    }
    

    
    

    
    
    
    
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PianoRollComponent)
};




