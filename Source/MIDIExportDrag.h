/*
  ==============================================================================

    MIDIExportDrag.h
    Created: 21 Feb 2022 12:19:07pm
    Author:  Axel Borgmo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include "BlockGridComponent.h"


using namespace juce;

//==============================================================================
/*
*/
class MIDIExportDrag : public juce::Component
//                                                            public juce::Component
//                                                    public juce::DragAndDropContainer,
                                                    
//                                                    public BlockGridControlPanel
//                                                    public Label
//                                                    public juce::DragAndDropContainer
//                                                    public Button::Listener
//                                                    public Slider::Listener,
//                                                    public Label::Listener
{


//    getSequence() = 0;
public:
    MIDIExportDrag();
    ~MIDIExportDrag();
    

    
    String _midiFilePath;
    
    void paint (Graphics& g) {
        Image image = juce::ImageCache::getFromMemory (BinaryData::midiExportIcon_png, BinaryData::midiExportIcon_pngSize);

        g.drawImage(image, getLocalBounds().reduced(10).toFloat());
    }
    
    void mouseDown (const MouseEvent &event) override; //{
        
//        DBG ("Clicked at: " << event.getPosition().toString());
//        startDragging("", this);
//    }
    
//    void mouseDrag (const MouseEvent& e) override {
//        std::cout << "TargetSource::mouseDrag():" << getX() << " " << getY() << " " << e.x << " " << e.y << std::endl;
//        DragAndDropContainer::performExternalDragDropOfFiles( { _midiFilePath }, false, nullptr, [ this ] ( void ) {
//                DBG( "clip dropped" );
            
//        });
//        juce::DragAndDropContainer* dragC = juce::DragAndDropContainer::findParentDragContainerFor(this);
        
//        if (!dragC) {
//            std::cout << "TargetSource::mouseDrag(): can't find parent drag container" << std::endl;
//        }
//        else {
//            if (!dragC->isDragAndDropActive())
//            {
//            dragC->startDragging("TargetSource", this);
//            }
//
//        }
//    }
    
private:
//    BlockGridComponent &blockGrid;
        
};

class TargetArea : public Component,
                   public DragAndDropTarget
{
public:
    void paint(Graphics &g) override
    {
        g.fillAll (Colours::green.withAlpha (0.2f));
    }

    bool isInterestedInDragSource (const SourceDetails& /*dragSourceDetails*/) override
    {
        return true;
    }

    void itemDragEnter (const SourceDetails& /*dragSourceDetails*/) override
    {
        std::cout << "DragAndDropCarrier():: itemDragEnter()" << std::endl;
        repaint();
    }

    void itemDragMove (const SourceDetails& /*dragSourceDetails*/) override
    {
        std::cout << "DragAndDropCarrier():: itemDragMove()" << std::endl;
    }

    void itemDragExit (const SourceDetails& /*dragSourceDetails*/) override
    {
        std::cout << "DragAndDropCarrier():: itemDragExit()" << std::endl;
        repaint();
    }

    void itemDropped (const SourceDetails& dragSourceDetails) override {
        if (dragSourceDetails.description == "TargetSource") {
            std::cout << "DragAndDropCarrier():: itemDropped(): valid item dropped" << std::endl;
        } else {
            std::cout << "DragAndDropCarrier():: itemDropped(): unknown container" << std::endl;
        }
    }
};


