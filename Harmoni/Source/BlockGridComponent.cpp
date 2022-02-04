/*
  ==============================================================================

    BlockGridComponent.cpp
    Created: 2 Feb 2022 1:40:01pm
    Author:  Axel Borgmo

  ==============================================================================
*/

#include "BlockGridComponent.h"
#include <array>

#ifndef LIB_VERSION
#include "DataLoggerRoot.h"
#endif


#define RETURN_IF_EDITING_DISABLED if(styleSheet.disableEditing) { return; }

//==============================================================================
BlockGridComponent::BlockGridComponent(BlockGridStyleSheet & ss) : gridStyle(ss)
{
    blackKeys.add(1);
    blackKeys.add(3);
    blackKeys.add(6);
    blackKeys.add(8);
    blackKeys.add(10);
    addChildComponent(&buttonBox);
    addKeyListener(this);
    setWantsKeyboardFocus(true);
    currentQValue = PRE::quantisedDivisionValues[PRE::eQuantisationValue1_32];
    lastNoteLength = PRE::quantisedDivisionValues[PRE::eQuantisationValue1_4];
    firstDrag = false;
    firstCall = false;
    lastTrigger = -1;
    ticksPerTimeSignature = PRE::defaultResolution * 4; //4/4 assume

}

BlockGridComponent::~BlockGridComponent()
{
    for (int i = 0; i < noteComps.size(); i++) {
        removeChildComponent(noteComps[i]);
        delete noteComps[i];
    }
}

void BlockGridComponent::paint (juce::Graphics& g)
{
    g.fillAll(Colours::darkgrey);
    
    const int totalBars = (getWidth() / pixelsPerBar) + 1;
    
    
    //draw piano roll background first.
    {
        float line = 0;//noteCompHeight;
        
        for (int i = 127; i >= 0; i--) {
            const int pitch = i % 12;
            g.setColour(blackKeys.contains(pitch) ?
                        Colours::darkgrey.withAlpha(0.5f) :
                        Colours::lightgrey.darker().withAlpha(0.5f));
         
            g.fillRect(0, (int)line, getWidth(), (int)noteCompHeight);
//            g.setColour(Colours::white);
//            g.drawText(String(i), 5, line, 40, noteCompHeight, Justification::left);
            
            line += noteCompHeight;
            g.setColour(Colours::black);
            g.drawLine(0, line, getWidth(), line);
        }
    }
    
    //again this is assuming 4/4
    const float increment = pixelsPerBar / 16;
    float line = 0;
    g.setColour(Colours::lightgrey);
    for (int i = 0; line < getWidth() ; i++) {
        float lineThickness = 1.0;
        if (i % 16 == 0) { //bar marker
            lineThickness = 3.0;
        }
        else if (i % 4 == 0) { //1/4 div
            lineThickness = 2.0;
        }
        g.drawLine(line, 0, line, getHeight(), lineThickness);
        
        line += increment;
    }
    
}

void BlockGridComponent::resized()
{
    
    for (auto component : noteComps) {
        if (component->coordiantesDiffer) {
            blockCompPositionMoved(component, false);
        }
        // convert from model representation into component representation (translation and scale)
        
        const float xPos = (component->getModel().getStartTime() / ((float) ticksPerTimeSignature)) * pixelsPerBar;
        const float yPos = (getHeight() - (component->getModel().getNote() * noteCompHeight)) - noteCompHeight;
        
        float len = (component->getModel().getNoteLegnth() / ((float) ticksPerTimeSignature)) * pixelsPerBar;
        
        component->setBounds(xPos, yPos, len, noteCompHeight);
    }

}


void BlockGridComponent::setupGrid (float px, float compHeight, const int bars)
{
    pixelsPerBar = px;
    noteCompHeight = compHeight;
    setSize(pixelsPerBar * bars, compHeight * 128); //we have 128 slots for notes
}


void BlockGridComponent::setQuantisation (const int val)
{
    if (val >= 0 && val <= PRE::eQuantisationValueTotal) {
        currentQValue = PRE::quantisedDivisionValues[val];
    }
    else {
        jassertfalse;
    }
}


//callback from PNoteComponent
void BlockGridComponent::blockCompSelected (KeyNoteComponent * nc, const MouseEvent& e)
{
    //RETURN_IF_EDITING_DISABLED
    
    int dragMove = 0;
    for (auto component : noteComps) {
        if (component->isMultiDrag) {
            dragMove++;
        }
    }
//    std::cout << "Drag: " << dragMove << "\n";
    
    for (auto component : noteComps) {
        if (component == nc) {
            component->setState(KeyNoteComponent::eSelected);
            component->toFront(true);
        }
        /*
         This complicated if statement exists because if the user is dragging multiple notes around we don't want to clear the selection.
         We only want so switch the selected note when the user selects another note
         */
        else if (component->getState() == KeyNoteComponent::eSelected && !e.mods.isShiftDown() && !dragMove) {
            component->setState(KeyNoteComponent::eNone);
        }
    }
    // need to reset the multidrag
    for (auto component : noteComps) {
        if (component->isMultiDrag) {
            component->isMultiDrag = false;
        }
    }
    sendEdit();
}

void BlockGridComponent::blockCompPositionMoved (KeyNoteComponent * comp, bool callResize)
{
    //RETURN_IF_EDITING_DISABLED
    
    if (!firstDrag) {

        firstDrag = true;
        // we want to move all the components...
        for (auto n : noteComps) {
            if (n != comp && n->getState() == KeyNoteComponent::eSelected) {
                blockCompPositionMoved(n, false);
            }
        }
        firstDrag = false;
        
    }
    
    //could do with refactoring this code here..
        int xPos = (comp->getX() / ((float)pixelsPerBar)) * ticksPerTimeSignature;
        int note = 127 - (comp->getY() / noteCompHeight);
        if (note > 127) {
            note = 127;
        }
        else if (note < 0) {
            note = 0;
        }
        
        if (xPos <= 0) {
            xPos = 0;
        }
        
        const int len = (comp->getWidth() / ((float)pixelsPerBar)) * ticksPerTimeSignature;
        NoteMessage nm = comp->getModel();
        nm.setNote(note);
        nm.setStartTime(xPos);
        nm.setNoteLegnth(len);
        nm.quantiseModel(currentQValue, true, true);
        nm.sendChange = sendChange;
        
        //todo: could make this toggleable behaviour
        lastNoteLength = nm.getNoteLegnth();
        
        comp->startY = -1;
        comp->startX = -1;
        comp->setValues(nm);
        if (callResize) {
            resized();
        }
        sendEdit();
    }

void BlockGridComponent::blockCompLengthChanged (KeyNoteComponent * original, int diff)
{
    //RETURN_IF_EDITING_DISABLED
    
    for (auto n : noteComps) {
        if (n->getState() == KeyNoteComponent::eSelected || n == original) {
            if (n->startWidth == -1) {
                n->startWidth = n->getWidth();
                n->coordiantesDiffer = true;
            }
            
            const int newWidth = n->startWidth - diff;
            // todo: this seems arbitary..
            if (newWidth > 20) {
                n->setSize(newWidth, n->getHeight());
            }
                
            
        }
    }
    sendEdit();
}




void BlockGridComponent::blockCompDragging (KeyNoteComponent* original, const MouseEvent& event)
{
    //RETURN_IF_EDITING_DISABLED
    
    for (auto n : noteComps) {
        if (n->getState() == KeyNoteComponent::eSelected && n != original) {
            
            const int movedX = event.getDistanceFromDragStartX();// (event.getx - original->startX);
            const int movedY = event.getDistanceFromDragStartY(); //(original->getY() - original->startY);
            
            if (n->startY == -1) {
                n->startX = n->getX();
                n->startY = n->getY();
            }
            
            /*
            std::cout << "Started at: " << n->startX << " - " << n->startY << "\n";
            std::cout << n->getBounds().toString() << "\n";
             */
            
            const int newX = n->startX + movedX;
            const int newY = n->startY + movedY;
            const int xDif = abs(newX - n->startX);
            const int yDif = abs(newY - n->startY);
            if (xDif > 2 || yDif > 2) { //ingnore a small amount of jitter.
                n->setTopLeftPosition(newX, newY);
                n->isMultiDrag = true;
            }
            
            /*
            std::cout << "Moved: " << movedX << " : " << movedY << " -- " << n->getX() << " : " << n->getY() <<  "\n" ;
            std::cout << n->getBounds().toString() << "\n \n" ;
             */

        }

    }
    

    /*
     This enables the notes to be triggered while dragging.
     */
    int note = 127 - (original->getY() / noteCompHeight);
    if (note > 127) { note = 127; }
    else if (note < 0) { note = 0; }
    if (note != lastTrigger) {
        original->getModel().trigger(note, 100);
        lastTrigger = note;
    }
    
}
void BlockGridComponent::setPositions ()
{
    //unused..
}

void BlockGridComponent::mouseDown (const MouseEvent&)
{
//    RETURN_IF_EDITING_DISABLED
    
    for (KeyNoteComponent * component : noteComps) {
        component->setState(KeyNoteComponent::eNone);
    }
    sendEdit();
}
void BlockGridComponent::mouseDrag (const MouseEvent& e)
{
    //RETURN_IF_EDITING_DISABLED
    
    if (!buttonBox.isVisible()) {
        buttonBox.setVisible(true);
        buttonBox.toFront(false);
        
        buttonBox.setTopLeftPosition(e.getPosition());
        buttonBox.startX = e.getPosition().x;
        buttonBox.startY = e.getPosition().y;
        
    }
    else {
        int xDir = e.getPosition().x - buttonBox.startX;
        int yDir = e.getPosition().y - buttonBox.startY;
        
        //work out which way to draw the selection box
        if (xDir < 0 && yDir < 0) { //top left
            buttonBox.setTopLeftPosition(e.getPosition().x, e.getPosition().y);
            buttonBox.setSize(buttonBox.startX - e.getPosition().getX(), buttonBox.startY - e.getPosition().getY());
        }
        else if (xDir > 0 && yDir < 0) { //top right
            buttonBox.setTopLeftPosition(buttonBox.startX, e.getPosition().y);
            buttonBox.setSize(e.getPosition().getX() - buttonBox.startX, buttonBox.startY - e.getPosition().getY());
        }
        else if (xDir < 0 && yDir > 0) { //bottom left
            buttonBox.setTopLeftPosition(e.getPosition().x, buttonBox.startY);
            buttonBox.setSize(buttonBox.startX - e.getPosition().getX(), e.getPosition().getY() -  buttonBox.startY);
        }
        else { //bottom right
            buttonBox.setSize(e.getPosition().getX() - buttonBox.getX(), e.getPosition().getY() - buttonBox.getY());
        }
    }
}
void BlockGridComponent::mouseUp (const MouseEvent&)
{
    //RETURN_IF_EDITING_DISABLED
    
    if (buttonBox.isVisible()) {
        
        
        for (KeyNoteComponent * component : noteComps) {
            if (component->getBounds().intersects(buttonBox.getBounds())) {
                component->setState(KeyNoteComponent::eState::eSelected);
            }
            else {
                component->setState(KeyNoteComponent::eState::eNone);
            }
        }
        buttonBox.setVisible(false);
        buttonBox.toFront(false);
        buttonBox.setSize(1,1);
    }
    
    sendEdit();
    
}

void BlockGridComponent::mouseDoubleClick (const MouseEvent& e)
{
//    RETURN_IF_EDITING_DISABLED
    
    const int xPos = (e.getMouseDownX() / ((float)pixelsPerBar)) * ticksPerTimeSignature;
    const int yIn = ((float)e.getMouseDownY() / noteCompHeight);
    const int note = 127 - yIn;
    jassert(note >= 0 && note <= 127);
    
    /*set up lambdas..
    
     Essentialy each note component (child) sends messages back to parent (this) through a series of lambda callbacks
     */
    
    KeyNoteComponent * nn = new KeyNoteComponent(gridStyle);
    nn->onNoteSelect = [this](KeyNoteComponent * n, const MouseEvent& e) {
        this->blockCompSelected(n, e);
    };
    nn->onPositionMoved = [this](KeyNoteComponent * n) {
        this->blockCompPositionMoved(n);
    };
    nn->onLegnthChange = [this](KeyNoteComponent * n, int diff) {
        this->blockCompLengthChanged(n, diff);
    };
    nn->onDragging = [this](KeyNoteComponent * n, const MouseEvent & e) {
        this->blockCompDragging(n, e);
    };
    addAndMakeVisible(nn);
    
    const int defaultVelocity = 100;
    
    NoteMessage nModel((u8)note, defaultVelocity, (st_int)xPos, lastNoteLength, {});
    nModel.quantiseModel(currentQValue, true, true);
    nModel.sendChange = sendChange;
    nModel.trigger();
    nn->setValues(nModel);
    

    noteComps.push_back(nn);
    
    resized();
    repaint();
    sendEdit();
}



bool BlockGridComponent::keyPressed (const KeyPress& key, Component* originatingComponent)
{
    
#ifndef LIB_VERSION
    LOG_KEY_PRESS(key.getKeyCode(), 1, key.getModifiers().getRawFlags());
#endif
    
    if (gridStyle.disableEditing) {
        return true;
    }
    if (key == KeyPress::backspaceKey) {
        //
        deleteAllSelected();
        sendEdit();
        return true;
    }
    else if (key == KeyPress::upKey || key == KeyPress::downKey) {
        bool didMove = false;
        for (auto nComp : noteComps) {
            if (nComp->getState() == KeyNoteComponent::eSelected) {
                NoteMessage nModel =  nComp->getModel();
                
                (key == KeyPress::upKey) ?
                nModel.setNote(nModel.getNote() + 1) :
                nModel.setNote(nModel.getNote() - 1);
                
                nModel.sendChange = sendChange;
                nComp->setValues(nModel);
                didMove = true;
            }
        }
        if (didMove) {
            sendEdit();
            resized();
            return true;
            
        }
        
    }
    else if (key == KeyPress::leftKey || key == KeyPress::rightKey) {
        bool didMove = false;
        const int nudgeAmount = currentQValue;
        for (auto nComp : noteComps) {
            if (nComp->getState() == KeyNoteComponent::eSelected) {
                NoteMessage nModel =  nComp->getModel();
                
                (key == KeyPress::rightKey) ?
                nModel.setStartTime(nModel.getStartTime() + nudgeAmount) :
                nModel.setStartTime(nModel.getStartTime() - nudgeAmount) ;
                
                nModel.sendChange = sendChange;
                nComp->setValues(nModel);
                didMove = true;
            }
        }
        if (didMove) {
            sendEdit();
            resized();
            return true;
        }
        
    }
    return false;
}

void BlockGridComponent::deleteAllSelected ()
{
    std::vector<KeyNoteComponent *> itemsToKeep;
    for (int i = 0; i < noteComps.size(); i++) {
        if (noteComps[i]->getState() == KeyNoteComponent::eSelected) {
            removeChildComponent(noteComps[i]);
            delete noteComps[i];
        }
        else {
            itemsToKeep.push_back(noteComps[i]);
        }
    }
    noteComps = itemsToKeep;
}

PRESequence BlockGridComponent::getSequence ()
{
    int leftToSort = (int) noteComps.size();
    
    std::vector<KeyNoteComponent *> componentsCopy = noteComps;
    /*
     inline lambda function to find the lowest startTime
     */
    auto findLowest = [&]() -> int {
        int lowestIndex = 0;
        for (int i = 0; i < componentsCopy.size(); i++) {
            if (componentsCopy[i]->getModel().getStartTime() < componentsCopy[lowestIndex]->getModel().getStartTime()) {
                lowestIndex = i;
            }
        }
        return lowestIndex;
    };
    
    
    PRESequence seq;
    while (leftToSort) {
        const int index = findLowest();
        auto m = componentsCopy[index]->getModel();
        m.flags.state = componentsCopy[index]->getState();
        seq.events.push_back(m);
//        seq.events[seq.events.size()-1]->flags =1  //we also want the selected flags..
        
        componentsCopy[index] = nullptr;
        componentsCopy.erase(componentsCopy.begin() + index);
        leftToSort--;
    }
    seq.print();
    return seq;
}

void BlockGridComponent::loadSequence (PRESequence sq)
{
    for (int i = 0; i < noteComps.size(); i++) {
        removeChildComponent(noteComps[i]);
        delete noteComps[i];
        
    }
    noteComps.clear();
    
    noteComps.reserve(sq.events.size());
    
    for (auto event : sq.events) {
        KeyNoteComponent * nn = new KeyNoteComponent(gridStyle);
        nn->onNoteSelect = [this](KeyNoteComponent * n, const MouseEvent& e) {
            this->blockCompSelected(n, e);
        };
        nn->onPositionMoved = [this](KeyNoteComponent * n) {
            this->blockCompPositionMoved(n);
        };
        nn->onLegnthChange = [this](KeyNoteComponent * n, int diff) {
            this->blockCompLengthChanged(n, diff);
        };
        nn->onDragging = [this](KeyNoteComponent * n, const MouseEvent & e) {
            this->blockCompDragging(n, e);
        };
        addAndMakeVisible(nn);
        NoteMessage nModel(event);
        nModel.sendChange = sendChange;
//        nModel.quantiseModel(PRE::defaultResolution / 8, true, true);
        nn->setValues(nModel);
        
        noteComps.push_back(nn);

    }
    resized();
    repaint();
}

float BlockGridComponent::getNoteCompHeight ()
{
    return noteCompHeight;
}

float BlockGridComponent::getPixelsPerBar ()
{
    return pixelsPerBar;
}

std::vector<NoteMessage *> BlockGridComponent::getSelectedMessages ()
{
    std::vector<NoteMessage *> noteMessages;
    for (auto comp : noteComps) {
        if (comp->getState()) {
           noteMessages.push_back(comp->getModelPtr());
        }
    }
    return noteMessages;
}

void BlockGridComponent::sendEdit ()
{
    if (this->onEdit != nullptr) {
        this->onEdit();
    }
}


