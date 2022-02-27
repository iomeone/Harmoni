/*
  ==============================================================================

    PianoRollComponent.cpp
    Created: 1 Feb 2022 9:00:03pm
    Author:  Axel Borgmo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PianoRollComponent.h"
#include "MIDIExportDrag.h"

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
    
    //
//    addAndMakeVisible(MIDIDragButton);
    

    

    
    
    
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
    scrollGrid.setBounds(80, 50 + 35, getWidth()-90, controlPanel.isVisible() ? getHeight()-180 : getHeight() - 55);
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
    controlPanel.setBounds(5, scrollGrid.getBottom() + 5, getWidth() - 10, 140 - 35);
    
//  menu
    menuBar.setBounds(0,0, getWidth(), 20 + 20);
    label.setBounds(10, 30, getWidth() - 20, 20);
    slider.setBounds(10,60, getWidth() - 20, 20);
    
    
//    MIDI out
//    MIDIDragButton.setBounds(controlPanel.getWidth() - 110, controlPanel.getBottom() - 115 , 100, 100);

    

}


//    menu
StringArray PianoRollComponent::getMenuBarNames() {
    const char* menuNames[] = { "File", "Similar Songs", 0 };
    
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
    else if (name =="Similar Songs")
    {
        menu.addItem(SliderMin, "Set to minimum");
        menu.addItem(SliderMax, "Set to maximum");
    }

    return menu;
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





//    menu
void PianoRollComponent::menuItemSelected(int menuID, int index)
{
    switch (menuID) {
        case midiImport:
            loadMIDIFile();
//            file.setText (String(), dontSendNotification);
            break;
            
        case midiExport:
            getSequence();
            break;
            
        case SliderMin:
            slider.setValue (slider.getMinimum());
            break;
            
        case SliderMax:
            slider.setValue(slider.getMaximum());
            break;
        
        
    }
}




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
