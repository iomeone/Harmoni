/*
  ==============================================================================

    PianoRollComponent.cpp
    Created: 1 Feb 2022 9:00:03pm
    Modified from Samuel Hunt: https://github.com/Sjhunt93/Piano-Roll-Editor

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PianoRollComponent.h"
#include <string>

//==============================================================================
PianoRollComponent::PianoRollComponent() :  noteGrid(gridStyle),
                                            controlPanel(noteGrid, gridStyle),
                                            menuBar (this)
{
    
 
    //--- grid
    addAndMakeVisible(scrollGrid);
 
    scrollGrid.setViewedComponent(&noteGrid, false);
    scrollGrid.setScrollBarsShown(true, true);
    scrollGrid.setScrollBarThickness(10);
 
    
    //--- timeline
    scrollNavbar.setViewedComponent(&navbarComp, false);
    scrollNavbar.setScrollBarsShown(false, false);
    addAndMakeVisible(scrollNavbar);
    
    //--- keyboard
    scrollKeyboard.setViewedComponent(&keyboardComp, false);
    scrollKeyboard.setScrollBarsShown(false, false);
    addAndMakeVisible(scrollKeyboard);
    
    
    //--- menu
    addAndMakeVisible(&menuBar);
    label.setEditable(true);
    
    //-- chord display
    addAndMakeVisible (chordDisplay);
    
    //--- chord library display
    addAndMakeVisible(chordLibrary);
    
    //once the piano roll component is scrolled then it updates the others manually
    scrollGrid.positionMoved = [this](int x, int y)
    {
        scrollNavbar.setViewPosition(x, y);
        scrollKeyboard.setViewPosition(x, y);
    };
    
    
    //--- controlPanel
    addAndMakeVisible(controlPanel);
    controlPanel.configureGrid = [this](int pixelsPerBar, int noteHeight)
    {
        setup(10, pixelsPerBar, noteHeight);
    };
    
    noteGrid.onEdit = [this]() //pass up the chain.
    {
        if (this->onEdit != nullptr) {
            this->onEdit();
        }
    };
    noteGrid.sendChange = [this](int note, int vel)
    {
        if (this->sendChange != nullptr) {
            this->sendChange(note, vel);
        }
    };
    showPlaybackMarker = false;
    playbackTicks = 0;
}

PianoRollComponent::~PianoRollComponent()
{
}

void PianoRollComponent::paint (juce::Graphics& g)
{
    g.fillAll(Colour(8, 12, 20));
    g.setFont(Font(14, 1));
    g.fillAll(Colour(8, 12, 20));
    
    chordDisplay.setColour (juce::TextEditor::backgroundColourId, (juce::Colours::transparentBlack));
    chordDisplay.setColour (juce::TextEditor::backgroundColourId, (juce::Colours::transparentBlack));
    chordDisplay.setColour (juce::TextEditor::outlineColourId, juce::Colour (juce::Colours::orange));
    chordDisplay.setColour (juce::TextEditor::shadowColourId, juce::Colour (juce::Colours::transparentBlack));
    chordDisplay.setColour (juce::Label::textColourId, Colours::black);
//    chordDisplay.getLookAndFeel().setColour (juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);
//    chordDisplay.getLookAndFeel().setColour (juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
}


//void PianoRollComponent::paintOverChildren (Graphics& g) // function for getting a moving playback cursor
//{
//    const int x = noteGrid.getPixelsPerBar() * (playbackTicks / (4.0 * PRE::defaultResolution));
//    const int xAbsolute = scrollGrid.getViewPosition().getX();
//
//    g.setColour(Colours::greenyellow);
//    g.drawLine(x - xAbsolute, 0, x - xAbsolute, getHeight(), 5.0);
//}


void PianoRollComponent::resized()
{
//    scrollGrid
    scrollGrid.setBounds(80, 50 + 35, getWidth()-90-400, controlPanel.isVisible() ? getHeight()-180 : getHeight() - 55);
    scrollNavbar.setBounds(scrollGrid.getX(), 5 + 35, scrollGrid.getWidth()-10, scrollGrid.getY() - 5);
    scrollKeyboard.setBounds(5, scrollGrid.getY(), 70, scrollGrid.getHeight()- 10);
    
//    noteGrid
    noteGrid.setBounds(0,0 + 35,4000, 20*127);
    noteGrid.setupGrid(900, 20, 10);
    
//    navbar
    navbarComp.setBounds(0, 0 + 35, 100, scrollNavbar.getHeight());
    navbarComp.setup(10, 900);
    
//    keyboard
    keyboardComp.setBounds(0, 0 + 35, scrollKeyboard.getWidth(), noteGrid.getHeight());
    
//    controlPanel
    controlPanel.setBounds(5, scrollGrid.getBottom() + 5, getWidth() - 10 - 400, 140 - 35);
    
//  menu
    menuBar.setBounds(0,0, scrollGrid.getWidth(), 20 + 20);
    label.setBounds(10, 30, getWidth() - 20 - 400, 20);
    slider.setBounds(10,60, getWidth() - 20 - 400, 20);
    
//    chord library
    chordLibrary.setBounds(1000, 0, 400, 800);
    
    
//    MIDI out
//    MIDIDragButton.setBounds(controlPanel.getWidth() - 110, controlPanel.getBottom() - 115 , 100, 100);
    
    
//    chordDisplay
//    auto quarterWidth = getWidth() / 4;
    chordDisplay.setBounds(getWidth() - 820, 0, 400, 40);
//    chordDisplay.setBounds(getLocalBounds().withWidth(quarterWidth), <#int y#>, <#int width#>, <#int height#>)
//    chordDisplay.setBounds (getLocalBounds().withWidth (quarterWidth).withX (quarterWidth).reduced (10));

    

}


//    menu
StringArray PianoRollComponent::getMenuBarNames() {
    const char* menuNames[] = { "File", "Chord Analysis", "Similar songs", 0 }; // "List of songs",
    
    return StringArray (menuNames);
}


//    menu
PopupMenu PianoRollComponent::getMenuForIndex(int index, const String &name) {
    PopupMenu menu;
    
    if (name == "File")
    {
        menu.addItem(midiImport, "Import MIDI");
        menu.addItem(midiExport, "Export MIDI");
    }
    else if (name =="Chord Analysis")
    {
        menu.addItem(similarSongs, "Find Similar Songs");
        menu.addItem(getChord, "Get Chord");
    }
    else if (name == "Similar songs")
    {
        menu.addItem(song1, "Song 1");
        menu.addItem(song2, "Song 2");
        menu.addItem(song3, "Song 3");
        menu.addItem(song4, "Song 4");
        menu.addItem(song5, "Song 5");
    }
    


    return menu;
}



//    menu
void PianoRollComponent::menuItemSelected(int menuID, int index)
{
    PRESequence seq;
    switch (menuID) {
        case midiImport: {
            loadMIDIFile();

            break; }
        case midiExport:{
            seq = getSequence();
            seq.exportMIDI();

            break;}
            
        case similarSongs: {
            seq = getSequence();
//            seq.searchSimilarSongs();
            String _chordProg = seq.searchSimilarSongs();
//            DBG("Chord prog is: " << _chordProg);
            logMessage(_chordProg);
            chordLibrary.searchChordLibrary(_chordProg);
//            chordLibrary.displayTopSongs(1);
//            seq.searchChordLib();
//            String chord_ = seq.searchSimilarSongs();
//            logMessage(chord_);
        
            break; }
            
        case getChord: {
            seq = getSequence();
//            seq.getChords();
            
            
            String chord_ = seq.getChordSymbol();
            logMessage(chord_);
            DBG("Chord_ is: " << chord_);
//            seq.
//            slider.setValue(slider.getMaximum());
            break;
        }
        
        case song1: {
//            seq = getSequence();
////            seq.searchSimilarSongs();
//            String _chordProg = seq.searchSimilarSongs();
////            DBG("Chord prog is: " << _chordProg);
//            logMessage(_chordProg);
//            chordLibrary.searchChordLibrary(_chordProg);
            chordLibrary.displayTopSongs(0);
            break;
        }
        case song2: {
//            seq = getSequence();
////            seq.searchSimilarSongs();
//            String _chordProg = seq.searchSimilarSongs();
////            DBG("Chord prog is: " << _chordProg);
//            logMessage(_chordProg);
//            chordLibrary.searchChordLibrary(_chordProg);
            chordLibrary.displayTopSongs(1);
            break;
        }
        case song3: {
//            seq = getSequence();
////            seq.searchSimilarSongs();
//            String _chordProg = seq.searchSimilarSongs();
////            DBG("Chord prog is: " << _chordProg);
//            logMessage(_chordProg);
//            chordLibrary.searchChordLibrary(_chordProg);
            chordLibrary.displayTopSongs(2);
            break;
        }
        case song4: {
//            seq = getSequence();
////            seq.searchSimilarSongs();
//            String _chordProg = seq.searchSimilarSongs();
////            DBG("Chord prog is: " << _chordProg);
//            logMessage(_chordProg);
//            chordLibrary.searchChordLibrary(_chordProg);
            chordLibrary.displayTopSongs(3);
            break;
        }
        case song5: {
//            seq = getSequence();
////            seq.searchSimilarSongs();
//            String _chordProg = seq.searchSimilarSongs();
////            DBG("Chord prog is: " << _chordProg);
//            logMessage(_chordProg);
//            chordLibrary.searchChordLibrary(_chordProg);
            chordLibrary.displayTopSongs(4);
            break;
        }
    }
}



void PianoRollComponent::loadMIDIFile() {
    using namespace juce;
    FileChooser chooser ("Choose a MIDI file", juce::File::getSpecialLocation(juce::File::userDesktopDirectory), "*.mid");
    
    
    
    if (chooser.browseForFileToOpen())
    {
        juce::File file = chooser.getResult();
        juce::FileInputStream inputStream(file);
        
        juce::MidiFile midifile;
        
        if (midifile.readFrom(inputStream))
        {
//            midifile.convertTimestampTicksToSeconds(); piano roll takes time stamps
            int trackToGet = 0;
            if (midifile.getNumTracks() > 1)
                jassert("Only one MIDI track can be imported at the time. ");
            
            std::vector<NoteMessage> midi_notes;
            const MidiMessageSequence *trackSequence = midifile.getTrack(trackToGet);
    
            for (int eventIndex = 0; eventIndex < trackSequence->getNumEvents(); eventIndex++) {
                //addNote()
                MidiMessageSequence::MidiEventHolder* noteEvent = trackSequence->getEventPointer(eventIndex);
                if (noteEvent->noteOffObject)
                {
                
                   u8 note = noteEvent->message.getNoteNumber();
                   u8 veloc = noteEvent->message.getVelocity() * 1.27;
                   st_int start = (noteEvent->message.getTimeStamp());
                   st_int end = (noteEvent->noteOffObject->message.getTimeStamp());
                   st_int length = end - start;
                    DBG("Initial start times: " << (int)start);
                    
                    
                    
                    midi_notes.push_back(NoteMessage((u8)note, veloc, (st_int)start, length, {}));
                    
                    
                }
            }
            PRESequence midi_seq;
            
            
            //make midi sequence from note message vector
            midi_seq.events = midi_notes;
//            DBG("midi seq.events in load midi file size: " << midi_seq.events.size());
            

            loadSequence(midi_seq);
//            getSequence();
            
        }
    }
}


//void PianoRollComponent::exportMIDIFile() {
//
//
//    File myMIDIFile = File::getSpecialLocation( File::SpecialLocationType::tempDirectory ).getChildFile( "Harmoni.mid");
//    MidiMessageSequence MIDISeq;
//
//    PRESequence vectorSeq = getSequence();
//    
//    for (auto nm : vectorSeq) {
//        std::cout << (int) nm.getNote() << " - " << (int) nm.getVelocity() << " : " << nm.getStartTime() << " - " << nm.getNoteLegnth() << "\n";
    
//    for (int eventIndex = 0; eventIndex < VectorSeq.size(); eventIndex++) {
//
//    }
    
    
    // My midisequence = MIDISeq
    
//    for(auto it = MIDIseq.begin(); MIDIseq != MIDIseq.end(); ++MIDIseq){
//                    const SequenceEntry& e = it->second;
//                    if(e._pos > seq->getLength()) break;
//                    ms.addEvent( MidiMessage::noteOn(1, e._note, e._vel).withTimeStamp(asTicks(e._pos + exportShift, 960)));
//                    ms.addEvent( MidiMessage::noteOff(1, e._note).withTimeStamp(asTicks(e._pos + e._duration + exportShift, 960)));
//                }
//}








void PianoRollComponent::showControlPanel (bool state)
{
    controlPanel.setVisible(state);
}

void PianoRollComponent::setup (const int bars, const int pixelsPerBar, const int noteHeight)
{
    
    if (bars > 1 && bars < 1000) { // sensible limits..

        noteGrid.setupGrid(pixelsPerBar, noteHeight, bars);
        navbarComp.setup(bars, pixelsPerBar);
        keyboardComp.setSize(scrollKeyboard.getWidth(), noteGrid.getHeight());
    }
    else {
        // you might be able to have a 1000 bars but do you really need too!?
        jassertfalse;
    }
}


void PianoRollComponent::updateBars (const int newNumberOfBars)
{
    if (newNumberOfBars > 1 && newNumberOfBars < 1000) { // sensible limits..
        const float pPb = noteGrid.getPixelsPerBar();
        const float nH = noteGrid.getNoteCompHeight();
        
        noteGrid.setupGrid(pPb, nH, newNumberOfBars);
        navbarComp.setup(newNumberOfBars, pPb);
        keyboardComp.setSize(scrollKeyboard.getWidth(), noteGrid.getHeight());
    }
    else {
        jassertfalse;
    }
}




void PianoRollComponent::loadSequence (PRESequence sequence)
{
    noteGrid.loadSequence(sequence);
    
    
    // fix me, this automatically scrolls the grid
//    const int middleNote = ((sequence.highNote - sequence.lowNote) * 0.5) + sequence.lowNote;
//    const float scrollRatio = middleNote / 127.0;
//    setScroll(0.0, scrollRatio);
}


PRESequence PianoRollComponent::getSequence ()
{
    return noteGrid.getSequence();
}


void PianoRollComponent::setScroll (double x, double y)
{
    scrollGrid.setViewPositionProportionately(x, y);
}
void PianoRollComponent::setPlaybackMarkerPosition (const st_int ticks, bool isVisable)
{
    showPlaybackMarker = isVisable;
    playbackTicks = ticks;
    repaint();
    
}
void PianoRollComponent::disableEditing (bool value)
{
    gridStyle.disableEditing = value;
    noteGrid.repaint();
}

BlockGridControlPanel & PianoRollComponent::getControlPanel ()
{
    return controlPanel;
}

PianoRollComponent::ExternalModelEditor PianoRollComponent::getSelectedNoteMessages ()
{
    ExternalModelEditor mEdit;
    mEdit.update = [this]()
    {
        noteGrid.resized();
        noteGrid.repaint();
    };
    mEdit.messages = noteGrid.getSelectedMessages();
    
    
//    std::string str(vec.begin(), vec.end());
    
//    std::string str(mEdit.messages.begin(), mEdit.messages.end());
//    logMessage(str);
//    for (auto i : mEdit.messages) {
//        DBG("Selected messages: " << i); }
//    DBG("get selected notes")
    
    
    return mEdit;
}


//
//
//void MIDIExportDrag::mouseDown(const MouseEvent &event) {
////
////    _midiFilePath = getSequence.getFullPathName();
//    
//    
//}
//
//
//void MIDIExportDrag::mouseDrag(const MouseEvent &event) {
////    DragAndDropContainer::performExternalDragDropOfFiles( { _midiFilePath }, false, nullptr, [ this ] ( void ) {
////            DBG( "clip dropped" );
////    });
//}
