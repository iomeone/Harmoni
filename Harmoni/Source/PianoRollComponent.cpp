/*
  ==============================================================================

    PianoRollComponent.cpp
    Created: 1 Feb 2022 9:00:03pm
    Author:  Axel Borgmo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PianoRollComponent.h"

//==============================================================================
PianoRollComponent::PianoRollComponent() : noteGrid(gridStyle), controlPannel(noteGrid, gridStyle)
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
    
    //once the piano roll component is scrolled then it updates the others manually
    scrollGrid.positionMoved = [this](int x, int y)
    {
        scrollNavbar.setViewPosition(x, y);
        scrollKeyboard.setViewPosition(x, y);
    };
    
    addAndMakeVisible(controlPannel);
    controlPannel.configureGrid = [this](int pixelsPerBar, int noteHeight)
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
    g.fillAll(Colours::darkgreen.darker());
}


void PianoRollComponent::paintOverChildren (Graphics& g)
{
    const int x = noteGrid.getPixelsPerBar() * (playbackTicks / (4.0 * PRE::defaultResolution));
    const int xAbsolute = scrollGrid.getViewPosition().getX();
    
    g.setColour(Colours::greenyellow);
    g.drawLine(x - xAbsolute, 0, x - xAbsolute, getHeight(), 5.0);
}


void PianoRollComponent::resized()
{
    scrollGrid.setBounds(80, 50, getWidth()-90, controlPannel.isVisible() ? getHeight()-180 : getHeight() - 55);
    scrollNavbar.setBounds(scrollGrid.getX(), 5, scrollGrid.getWidth()-10, scrollGrid.getY() - 5);
    scrollKeyboard.setBounds(5, scrollGrid.getY(), 70, scrollGrid.getHeight()- 10);
    
    noteGrid.setBounds(0,0,4000, 20*127);
    noteGrid.setupGrid(900, 20, 10);
    navbarComp.setBounds(0, 0, 100, scrollNavbar.getHeight());
    navbarComp.setup(10, 900);
    keyboardComp.setBounds(0, 0, scrollKeyboard.getWidth(), noteGrid.getHeight());
    
    controlPannel.setBounds(5, scrollGrid.getBottom() + 5, getWidth() - 10, 140);

}


void PianoRollComponent::showControlPanel (bool state)
{
    controlPannel.setVisible(state);
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

BlockGridControlPanel & PianoRollComponent::getControlPannel ()
{
    return controlPannel;
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
    return mEdit;
}

