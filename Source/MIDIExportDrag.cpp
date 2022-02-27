/*
  ==============================================================================

    MIDIExportDrag.cpp
    Created: 21 Feb 2022 12:19:07pm
    Author:  Axel Borgmo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MIDIExportDrag.h"


//==============================================================================
MIDIExportDrag::MIDIExportDrag ()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.


}


MIDIExportDrag::~MIDIExportDrag(){}

//void mouseDrag(const MouseEvent &event) {
//    DragAndDropContainer::performExternalDragDropOfText("HALLA");
////    DragAndDropContainer::performExternalDragDropOfFiles( {_midiFilePath }, false, nullptr [this] (void) {
//        DBG("clip dropped");
////    })
//}

//void exportMIDIFile() {
//
//    // Generate MIDI file
//            File outf = File::getSpecialLocation( File::SpecialLocationType::tempDirectory ).getChildFile( "Harmoni.mid" );
//}

//MIDIExportDrag::~MIDIExportDrag()
//{
//}

//void MIDIExportDrag::paint (juce::Graphics& g)
//{
//    /* This demo code just fills the component's background and
//       draws some placeholder text to get you started.
//
//       You should replace everything in this method with your own
//       drawing code..
//    */
//
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
//
//    g.setColour (juce::Colours::grey);
//    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
//
//    g.setColour (juce::Colours::white);
//    g.setFont (14.0f);
//    g.drawText ("MIDIExportDrag", getLocalBounds(),
//                juce::Justification::centred, true);   // draw some placeholder text
//}

//void MIDIExportDrag::resized()
//{
//    // This method is where you should set the bounds of any child
//    // components that your component contains..
//
//}


