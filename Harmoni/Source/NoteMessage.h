/*
  ==============================================================================

    NoteMessage.h
    Created: 2 Feb 2022 3:57:45pm
    Author:  Axel Borgmo

  ==============================================================================
*/

#pragma once

// not sure if needed #include <JuceHeader.h>
#include "PConstants.h"
#include <vector>
#include <iostream>

//==============================================================================
/*
*/
class NoteMessage { // : public juce::Component { he dind't have it
public:
    
    struct Flags {
        Flags ()
        {
            state = 0;
            isGenerative = 0;
        }
        
        unsigned int state : 2;
        unsigned int isGenerative : 2; // skjønner ikke syntaxen
    };
    
    
    NoteMessage();
    NoteMessage (u8 n, u8 v, st_int st, st_int nl, Flags flags);
    // Han har ingen deconstructor igjen
    NoteMessage(const NoteMessage & other);// hadde ikke: override;
    
    void quantiseModel (int qValue, bool qStartTime, bool qNoteLegnth);
    
    bool compare (const NoteMessage & other, bool compareUIDs = true);

    // getters and setters
    // setters also trigger notes and register with the interaction logger (IGME only)
    void setNote (u8 _note);
    void setVelocity (u8 _velocity);
    void setStartTime (st_int _time);
    void setNoteLegnth (st_int _len);
    
    u8 getNote () {return note;}
    u8 getVelocity () {return velocity;}
    st_int getStartTime () {return startTime;}
    st_int getNoteLegnth () {return noteLegnth;}
    
    
    Flags flags; //the first 8 bits are for custom colours that you might want to map.
    
    std::function<void(int note,int velocity)> sendChange;
    void trigger();
    void trigger(const u8 note, const u8 vel);
    
    
#ifndef LIB_VERSION
    int64_t        uniqueId;
#endif

private:
    u8 note;
    u8 velocity;
    st_int startTime;
    st_int noteLegnth;
    
    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteMessage)
};


class PRESequence { //Piano Roll Editor Sequence
public:
    std::vector<NoteMessage> events;
    int tsLow;
    int tsHight;
    
    int lowNote;
    int highNote;
    
    /*
     used to debug
     */
    void print ();
};
