/*
  ==============================================================================

    KeyNoteComponent.cpp
    Created: 2 Feb 2022 2:39:01pm
    Author:  Axel Borgmo

  ==============================================================================
*/

//#include <JuceHeader.h> han hadde ikke
#include "KeyNoteComponent.h"

//==============================================================================
KeyNoteComponent::KeyNoteComponent (BlockGridStyleSheet & ss) : gridStyle(ss), edgeResizer(this, nullptr, ResizableEdgeComponent::Edge::rightEdge)
{
    mouseOver = useCustomColour = false;
    addAndMakeVisible(edgeResizer);
    setMouseCursor(normal);
    startWidth = startX = startY = -1;
    coordiantesDiffer = false;
    isMultiDrag = false;
    state = eNone;
    
    setCustomColour(Colours::orange);
    
    
}

KeyNoteComponent::~KeyNoteComponent()
{
}

void KeyNoteComponent::paint (Graphics & g)
{
    Colour orange(253,188,64);
    orange = orange.brighter();
    Colour red(252,97,92);
    
    g.fillAll(Colours::darkgrey); //border...
    Colour cToUse;
    if (useCustomColour && message.flags.isGenerative) {
        cToUse = customColour;
    }
    else {
//        cToUse = Colour::fromHSV(model.velocity/127.0, 1.0, 0.5, 0xFF);
        //cToUse = orange.interpolatedWith(red, model.velocity / 127.0);
        cToUse = red;
    }
    
    
    
    if (state || mouseOver) {
        cToUse = cToUse.brighter().brighter();
    }
    g.setColour(cToUse);
    
    //draw middle box.
    g.fillRect(1, 1, getWidth() - 2, getHeight() - 2);

    
    //draw velocity
    if (getWidth() > 10 && gridStyle.getDrawVelocity()) {
        g.setColour(cToUse.brighter());
        const int lineMax = getWidth() - 5;
        
        g.drawLine(5, getHeight() * 0.5 - 2, lineMax * (message.getVelocity()/127.0), getHeight() * 0.5 - 2, 4);
    }
    String toDraw;
    if (gridStyle.getDrawMIDINoteStr()) {
        toDraw += String(PRE::pitches_names[message.getNote() % 12]) + String(message.getNote() / 12) + String(" ");
    }
    if (gridStyle.getDrawMIDINum()) {
        toDraw += String(message.getNote());
    }
    
    g.setColour(Colours::white);
    g.drawText(String(toDraw), 3, 3, getWidth() - 6, getHeight() - 6, Justification::centred);
    
}

void KeyNoteComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void KeyNoteComponent::setCustomColour (Colour c)
{
    customColour = c;
    useCustomColour = true;
}


void KeyNoteComponent::setValues (NoteMessage m)
{
    
    if (m.getNote() == 255) { m.setNote(0); } //unsigned overflow
    if (m.getNote() > 127) { m.setNote(127); }
    
    //cast to int as noteLen is unsigned. allows us to check for 0
    if (((int)m.getStartTime()) < 0) { m.setStartTime(0); }
    
    message = m;
    repaint();
//    DBG("setValues function: " << (int)m.getStartTime());

}


NoteMessage KeyNoteComponent::getModel ()
{
    return message;
}



NoteMessage * KeyNoteComponent::getModelPtr ()
{
    return &message;
}

void KeyNoteComponent::setState (eState s)
{
    state = s;
    repaint();
}
KeyNoteComponent::eState KeyNoteComponent::getState ()
{
    return state;
}

void KeyNoteComponent::mouseEnter (const MouseEvent&)
{
    //CHECK_EDIT don't know how he get those orange texts for these
    mouseOver = true;
    repaint();
}
void KeyNoteComponent::mouseExit  (const MouseEvent&)
{
    //CHECK_EDIT
    mouseOver = false;
    setMouseCursor(MouseCursor::NormalCursor);
    repaint();
    
}
void KeyNoteComponent::mouseDown (const MouseEvent& e)
{
    //CHECK_EDIT

    if (e.mods.isShiftDown()) {
        velocityEnabled = true;
        startVelocity = message.getVelocity();
    }
    else if (getWidth() - e.getMouseDownX() < 10) {
        resizeEnabled = true;
        startWidth = getWidth();
    }
    else {
        startDraggingComponent(this, e);
        
    }
    if (!resizeEnabled) {
        
    }
}
void KeyNoteComponent::mouseUp (const MouseEvent& e)
{
    //CHECK_EDIT
    if (onPositionMoved != nullptr) {
        onPositionMoved(this);
    }
    if (onNoteSelect != nullptr) {
        onNoteSelect(this, e);
    }
    startWidth = startX = startY -1;
    mouseOver = false;
    resizeEnabled = false;
    velocityEnabled = false;
    repaint();
    isMultiDrag = false;
    
}
void KeyNoteComponent::mouseDrag  (const MouseEvent& e)
{
    //CHECK_EDIT
    //velocityEnabled
    if (resizeEnabled) {
        if (onLegnthChange != nullptr) {
            onLegnthChange(this, startWidth-e.getPosition().getX());
        }

    }
    else if (velocityEnabled) {
        int velocityDiff = e.getDistanceFromDragStartY() * -0.5;
        int newVelocity = startVelocity + velocityDiff;
        if (newVelocity < 1) {
            newVelocity = 1;
        }
        else if (newVelocity > 127) {
            newVelocity = 127;
        }
        message.setVelocity(newVelocity);
        repaint();
//        std::cout << velocityDiff << "\n";
        
    }
    else {
        setMouseCursor(MouseCursor::DraggingHandCursor);
        dragComponent(this, e, nullptr);
        
        if (onDragging != nullptr ) { //&& isMultiDrag
            onDragging(this, e);
        }
    }
    
}
void KeyNoteComponent::mouseMove  (const MouseEvent& e)
{
    //CHECK_EDIT
    if (getWidth() - e.getMouseDownX() < 10) {
        setMouseCursor(MouseCursor::RightEdgeResizeCursor);
    }
    else {
        setMouseCursor(MouseCursor::NormalCursor);
    }
}


