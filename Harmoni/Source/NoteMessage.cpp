/*
  ==============================================================================

    NoteMessage.cpp
    Created: 2 Feb 2022 3:57:45pm
    Author:  Axel Borgmo

  ==============================================================================
*/

#include "NoteMessage.h"
#ifndef LIB_VERSION
#include "StaticCounter.h"
#include "DataLoggerRoot.h"
#endif

//==============================================================================
NoteMessage::NoteMessage()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    note = 0;
    velocity = 0;
    startTime = 0;
    noteLegnth = 0;
#ifndef LIB_VERSION // jeg vet ikke hva dette betyr
    uniqueId = StaticCounter::count();
#endif
}


NoteMessage::NoteMessage (u8 n, u8 v, st_int st, st_int nl, Flags f)
{
    note = n;
    velocity = v;
    startTime = st;
    noteLegnth = nl;
    flags = f;
#ifndef LIB_VERSION
    uniqueId = StaticCounter::count();
#endif
}


NoteMessage::NoteMessage (const NoteMessage & other)
{
    note = other.note;
    velocity = other.velocity;
    startTime = other.startTime;
    noteLegnth = other.noteLegnth;
    flags = other.flags;
#ifndef LIB_VERSION
    uniqueId = other.uniqueId;
#endif
    sendChange = other.sendChange;
}

//NoteMessage::~NoteMessage() han hadde ikke constructor
//{
//}

/*
 Quantising to say 1/8 notes would require an input value of 240 (or 0.5 * the default resolution)
 */
void NoteMessage::quantiseModel (int qValue, bool qStartTime, bool qNoteLegnth)
{
    auto quantiseValue = [&](int qDiv, int valueToQuantise) -> int
    {
        const int simpleQLow = (valueToQuantise / qDiv) * qDiv; //use lossey int division
        const int simpleQHigh = ((valueToQuantise / qDiv)+1) * qDiv;
        
        if (valueToQuantise-simpleQLow < simpleQHigh-valueToQuantise) { //lower boundry
            return simpleQLow;
        }
        return simpleQHigh;
    };
    
    if (qStartTime) {
        startTime = quantiseValue(qValue, startTime);
    }
    if (qNoteLegnth) {
        noteLegnth = quantiseValue(qValue, noteLegnth);
        if (noteLegnth == 0) {
            noteLegnth = qValue; //
        }
    }
    
}




bool NoteMessage::compare (const NoteMessage & other, bool compareUIDs)
{
#define RETURN_IF(A) if(A){return false;}
    
    RETURN_IF(note                  != other.note)
    RETURN_IF(velocity              != other.velocity)
    RETURN_IF(startTime             != other.startTime)
    RETURN_IF(noteLegnth            != other.noteLegnth)
    RETURN_IF(flags.state           != other.flags.state)
    RETURN_IF(flags.isGenerative    != other.flags.isGenerative)
    
#ifndef LIB_VERSION
    if (compareUIDs) {
        RETURN_IF(uniqueId != other.uniqueId)
    }
#endif
    return true;
}


void NoteMessage::setNote (u8 _note)
{
    note = _note;
    trigger();
#ifndef LIB_VERSION
    LOG_NOTE_EDITED_PR(note, velocity, startTime, noteLegnth);
#endif
}


void NoteMessage::setVelocity (u8 _velocity)
{
    velocity = _velocity;
    trigger();
#ifndef LIB_VERSION
    LOG_NOTE_EDITED_PR(note, velocity, startTime, noteLegnth);
#endif
}
void NoteMessage::setStartTime (st_int _time)
{
    startTime = _time;
}
void NoteMessage::setNoteLegnth (st_int _len)
{
    noteLegnth = _len;
}

void NoteMessage::trigger()
{
    if (sendChange != nullptr) {
        sendChange(note, velocity);
    }
}
void NoteMessage::trigger(const u8 n, const u8 vel)
{
    if (sendChange != nullptr) {
        sendChange(n, vel);
    }
}
void PRESequence::print ()
{
    for (auto nm : events) {
        std::cout << (int) nm.getNote() << " - " << (int) nm.getVelocity() << " : " << nm.getStartTime() << " - " << nm.getNoteLegnth() << "\n";
    }
}


