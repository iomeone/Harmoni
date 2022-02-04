/*
  ==============================================================================

    KeyNoteComponent.h
    Created: 2 Feb 2022 2:39:01pm
    Author:  Axel Borgmo

  ==============================================================================
*/

#pragma once

// not sure if needed #include <JuceHeader.h>

#include "PConstants.h"
#include "NoteMessage.h" 
#include "BlockGridStyleSheet.h"

//==============================================================================
/*
*/
class KeyNoteComponent  : public juce::Component, public ComponentDragger
{
public:
    
    enum eState {
        eNone,
        eSelected,
    };
    
    struct MultiEditState {
        int startWidth; //used when resizing the noteComponents length RENAME COMMENT
        bool coordinatesDiffer; //sometimes the size of this component gets changed externally (for example on multi-resizing) set this flag to be true and at
        Rectangle<int> startingBounds;
    };
    
    KeyNoteComponent(BlockGridStyleSheet & gridStyle);
    ~KeyNoteComponent();

    void paint (juce::Graphics&) override; //he did not have override here
    void resized() override; // or here
    void setCustomColour (Colour c);
    
    void setValues (NoteMessage model);
    NoteMessage getModel ();
    NoteMessage * getModelPtr();
    
    void setState (eState state);
    eState getState ();
    
    void mouseEnter (const MouseEvent&) override;
    void mouseExit  (const MouseEvent&) override;
    void mouseDown  (const MouseEvent&) override;
    void mouseUp    (const MouseEvent&) override;
    void mouseDrag  (const MouseEvent&) override;
    void mouseMove  (const MouseEvent&) override;
    
    
    std::function<void(KeyNoteComponent*, const MouseEvent&)> onNoteSelect;
    std::function<void(KeyNoteComponent*)> onPositionMoved;
    std::function<void(KeyNoteComponent*, const MouseEvent&)> onDragging; //send the drag event to other components..
    std::function<void(KeyNoteComponent*, int)> onLegnthChange; //sends the difference as this is relative for all components.
    
    int minWidth = 10;
    int startWidth; //used when resizing the noteComponents length
    int startX, startY;
    bool coordiantesDiffer; //sometimes the size of this component gets changed externally (for example on multi-resizing) set this flag to be true and at some point the internal model will get updated also
    bool isMultiDrag;

private:
    
    BlockGridStyleSheet & gridStyle;
    ResizableEdgeComponent edgeResizer;
    
    bool mouseOver, useCustomColour, resizeEnabled, velocityEnabled;
    int startVelocity;
    
    Colour customColour;
    NoteMessage message;
    MouseCursor normal;
    eState state;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyNoteComponent)
};
