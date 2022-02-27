/*
  ==============================================================================

    BlockGridControlPanel.cpp
    Created: 2 Feb 2022 1:42:10pm
    Author:  Axel Borgmo

  ==============================================================================
*/

//#include <JuceHeader.h> han hadde ikke
#include "BlockGridControlPanel.h"

//==============================================================================
BlockGridControlPanel::BlockGridControlPanel (BlockGridComponent & component, BlockGridStyleSheet & ss) : blockGrid(component), gridStyle(ss)
{
    addAndMakeVisible(noteCompHeight);
    addAndMakeVisible(pixelsPerBar);
    
    // These are all arbitary values
    noteCompHeight.setRange(10, 30, 1);
    pixelsPerBar.setRange(100, 2000, 1);
    
    pixelsPerBar.setTextValueSuffix(" Pixels per bar");
    noteCompHeight.setTextValueSuffix(" Pixels per row");
    
    
    pixelsPerBar.setValue(900, dontSendNotification);
    noteCompHeight.setValue(20, dontSendNotification);
    
    pixelsPerBar.onValueChange = [this]()
    
    
    {
        if (configureGrid) {
            configureGrid(pixelsPerBar.getValue(), noteCompHeight.getValue());
        }
//        noteGrid.setupGrid(pixelsPerBar.getValue(), noteCompHeight.getValue());
//        noteGrid.repaint();
    };
    noteCompHeight.onValueChange = pixelsPerBar.onValueChange;
    
    
    addAndMakeVisible(drawMIDINotes);
    addAndMakeVisible(drawMIDIText);
    addAndMakeVisible(drawVelocity);
    addAndMakeVisible(render);
//    addAndMakeVisible(MIDIDragButton);
    
//    render.paintButton(Graphics &g, true, false) {
//        render.paint(juce::Graphics &)
//    }
    
    render.setButtonText("Export MIDI");
    drawMIDINotes.setButtonText("Draw MIDI Notes");
    drawMIDIText.setButtonText("Draw MIDI Text");
    drawVelocity.setButtonText("Draw Velocity");
    
    render.onClick = [this]()
    {
//        renderSequence();
        blockGrid.getSequence();
    };
    
    drawMIDIText.onClick = [this]()
    {
        gridStyle.drawMIDINoteStr = drawMIDIText.getToggleState();
        gridStyle.drawMIDINum = drawMIDINotes.getToggleState();
        gridStyle.drawVelocity = drawVelocity.getToggleState();
        blockGrid.repaint();
    };
    drawVelocity.onClick = drawMIDINotes.onClick = drawMIDIText.onClick;
    
    addAndMakeVisible(quantisationValue);
    quantisationValue.addItem("1/64", PRE::eQuantisationValueNone+1);
    quantisationValue.addItem("1/32", PRE::eQuantisationValue1_32+1);
    quantisationValue.addItem("1/16", PRE::eQuantisationValue1_16+1);
    quantisationValue.addItem("1/8",  PRE::eQuantisationValue1_8+1);
    quantisationValue.setSelectedItemIndex(1);
    
    quantisationValue.onChange = [this]()
    {
        blockGrid.setQuantisation(quantisationValue.getSelectedItemIndex());
    };
}
BlockGridControlPanel::~BlockGridControlPanel ()
{
    
}

//PRESequence BlockGridControlPanel::getSequence()
//{
//    blockGrid.getSequence();
//}




//virtual void Button::paintButton(Graphics &g, true, false) {
//    Image image = juce::ImageCache::getFromMemory (BinaryData::midiExportIcon_png, BinaryData::midiExportIcon_pngSize);
//
//    g.drawImage(image, getLocalBounds().reduced(10).toFloat());
//}

void BlockGridControlPanel::setQuantisation (PRE::eQuantisationValue value)
{
    quantisationValue.setSelectedItemIndex(value);
}


void BlockGridControlPanel::resized ()
{
    pixelsPerBar.setBounds(5, 5, 300, (getHeight() / 2) - 20);
    noteCompHeight.setBounds(5, pixelsPerBar.getBottom() + 5, 300, (getHeight() / 2) - 20);
    
    pixelsPerBar.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, false, 150, pixelsPerBar.getHeight() - 5);
    noteCompHeight.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, false, 150, noteCompHeight.getHeight() - 5);
    
    
    drawMIDINotes.setBounds(pixelsPerBar.getRight() + 5, 5, 150, (getHeight() / 3) - 10);
    drawMIDIText.setBounds(pixelsPerBar.getRight() + 5, drawMIDINotes.getBottom() + 5, 200, drawMIDINotes.getHeight());
    drawVelocity.setBounds(pixelsPerBar.getRight() + 5, drawMIDIText.getBottom() + 5, 200, drawMIDINotes.getHeight());
    
    render.setBounds(getWidth() - 100, 5, 95, 40);
    quantisationValue.setBounds(drawMIDINotes.getRight() + 5, 5, 200, drawMIDINotes.getHeight());
    
//    MIDIDragButton.setBounds(getWidth() - 120, -12 , 100, 100);
    
}
void BlockGridControlPanel::paint (Graphics & g)
{
    
}

void BlockGridControlPanel::renderSequence ()
{
    //warning this needs to be moved..
}

//
//
void MIDIExportDrag::mouseDown(const MouseEvent &event){

//    _midiFilePath = PRESequence.exportMidiFile().getFullPathName();
//    _midiFilePath = noteGrid.getSequence().getFullPathName();
//    DBG("Her skal jeg få midifilepath");
//    PRESequence getSequence();
//    auto _sequence = dynamic_cast<BlockGridComponent*>(getParentComponent());

//    _sequence->getSequence();
//    blockGrid.getSequence();
}

