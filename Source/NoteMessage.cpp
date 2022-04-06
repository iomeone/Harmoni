/*
  ==============================================================================

    NoteMessage.cpp
    Created: 2 Feb 2022 3:57:45pm
    Modified from Samuel Hunt: https://github.com/Sjhunt93/Piano-Roll-Editor

  ==============================================================================
*/

#include "NoteMessage.h"
#ifndef LIB_VERSION
#include "StaticCounter.h"
#include "DataLoggerRoot.h"
#include <fstream>
#include <vector>
#include <algorithm>


#endif

//==============================================================================
NoteMessage::NoteMessage()
{

    
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
//    DBG("NoteMessage function " << note);
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
//        DBG("note from trigger function" << note);
    }
}
void NoteMessage::trigger(u8 n, const u8 vel)
{
    if (sendChange != nullptr) {
        sendChange(n, vel);
    }
}



File PRESequence::exportMIDI() {
    FileChooser chooser ("Select a directory", juce::File::getSpecialLocation(juce::File::userDesktopDirectory), "*.mid");
//    File outf = File::getSpecialLocation( File::SpecialLocationType::tempDirectory ).getChildFile( "file.mid" );
    
    chooser.browseForFileToSave(true);
    
    juce::File outf = chooser.getResult();
    
    MidiFile myMIDIFile;

    MidiMessageSequence myMIDISequence;
    
    myMIDISequence.addEvent(MidiMessage::textMetaEvent(3, "Harmoni Chords"));
    myMIDISequence.addEvent(MidiMessage::timeSignatureMetaEvent(4, 4)); //assuming 4/4
    // myMIDISequence.addEvent(MidiMessage::tempoMetaEvent(microsecondsperquarter)); here I can add tempo
    DBG("Export gets called");
    
    

        for (auto nm : events) {
            myMIDISequence.addEvent(MidiMessage::noteOn(1, nm.getNote(), nm.getVelocity()).withTimeStamp(nm.getStartTime()));
            myMIDISequence.addEvent(MidiMessage::noteOff(1, nm.getNote()).withTimeStamp(nm.getStartTime() + nm.getNoteLegnth()));
    //        DBG("Note export start time:" << (float)nm.getStartTime());
    //        DBG("Note export length:" << (float)nm.getNoteLegnth());
    //        DBG("");
           
        //MidiMessage::noteOn(1 /*channel */), nm.getNote(), nm.getVelocity(), nm.getStartTime());
        
    //        std::cout << (int) nm.getNote() << " - " << (int) nm.getVelocity() << " : " << nm.getStartTime() << " - " << nm.getNoteLegnth() << "\n";
    //        DBG("Get sequence numbers. Note: " << (int)nm.getNote());
        }
    //    DBG(" nm.events in export function size" << events.size());
        
        myMIDIFile.setTicksPerQuarterNote( 960 );
        myMIDIFile.addTrack(myMIDISequence);
        myMIDISequence.addEvent(MidiMessage::endOfTrack());
        

        if (std::unique_ptr<FileOutputStream> p_os = outf.createOutputStream() ) {
            p_os->setPosition(0);
            p_os->truncate();
            myMIDIFile.writeTo(*p_os, 0);
            p_os->flush();
    //    DBG("Write to midifile has been called");
        
        }
    
    
    return outf;
    
}


String PRESequence::getNoteName(int intNote)
{
    intNote = intNote % 12;
//    DBG("intNote is: " << intNote);
    String noteString;
    
    
//    DBG(r);
//    DBG(root);

    
        switch (intNote) {
            case 0:
                noteString = "C";
                break;
            case 1:
                noteString = "Db";
//                DBG("Root string is: " << rootString);
                break;
            case 2:
                noteString = "D";
                break;
            case 3:
                noteString = "Eb";
                break;
            case 4:
                noteString = "E";
                break;
            case 5:
                noteString = "F";
                break;
            case 6:
                noteString = "Gb";
                break;
            case 7:
                noteString = "G";
                break;
            case 8:
                noteString = "Ab";
                break;
            case 9:
                noteString = "A";
                break;
            case 10:
                noteString = "Bb";
                break;
            case 11:
                noteString = "B";
                break;
        }
    return noteString;
}
    



String PRESequence::getChordSymbol()
{
//    DBG("====================================");
    int mod;
    
    noteNumbers.clear();
    intervals.clear();
//    noteNumbers.clear();

    
    
    
    
    // intervals vector stores unique intervals
//    std::vector<int> intervals;
//    template <typename T>
//    bool contains()
    if (events.size() < 3) // less than 3 notes
    {
        DBG("Less than 3 notes");
        return String("Less than 3 notes");
    }
    

    for (auto nm : events) { // place note names in noteNumber vector
        int note = nm.getNote();
        mod = note;
        noteNumbers.emplace_back (note);
        
    }
    
    std::sort (noteNumbers.begin(), noteNumbers.end()); // sort noteNumbers

    int bassNoteInt = noteNumbers[0]; // lowest note is the bassnote
    bassNote = getNoteName(bassNoteInt);
    
    for (auto i : noteNumbers) {
        int difference = (i - bassNoteInt) % 12; // get the interval difference from the lowest note and apply modolus

        if (difference != 0 && std::find (noteNumbers.begin(), noteNumbers.end(), difference) == noteNumbers.end())
        {

            intervals.emplace_back (difference);
//            DBG("Note added: " << difference);
        }
    }
    
    
//    getChordMatch

    auto _intervals = chordNumbMatch.find(intervals);

    
//    for (int i = 0; intervals.size(); ++i) {
//        DBG("Going through for loop");
//        DBG("intervals is: " << i);
//    }
    



    
    
//    for (auto nm : events) { // get note, mod and remove duplicates.
//        int note = nm.getNote();
//        note = note % 12;
//        noteNumbers.push_back(note);
////        root = note
////        DBG(nm.getNote());
//
////        mod = note % 12; // modulus getting only 1-12 values.
//
////        if (std::find (noteNumbers.begin(), noteNumbers.end(), mod) == noteNumbers.end()) // mod != 0 &&  at start?
////        {
////            noteNumbers.emplace_back (mod);
////        }
//
////        DBG("Modulo number: " << mod);
//    }
    
    DBG("NoteNumbers[0]: " << noteNumbers[0]);
    DBG("NoteNumbers[1]: " << noteNumbers[1]);
    DBG("NoteNumbers[2]: " << noteNumbers[2]);
    DBG("NoteNumbers[3]: " << noteNumbers[3]);
    DBG("NoteNumbers[4]: " << noteNumbers[4]);
    
    
    if (_intervals == chordNumbMatch.end())
    {
        DBG("Cannot recognize this chord yet. ");
        return String("Cannot recognize this chord yet. ");
    }

    String root;
    String chordSymbol;
    

    
    
    switch (_intervals->second)
    {
        //-----TRIADS MAJOR---------------------------------------
        case ChordTypes::MajorTriad:
            root = getNoteName(noteNumbers[0]);
            chordSymbol = (root + " Major Triad");
//            DBG(chordSymbol);
            return chordSymbol;
        case ChordTypes::MajorTriad3rdBass:
            root = getNoteName(noteNumbers[2]);
            chordSymbol = (root + " Major Triad / " + bassNote);
            return chordSymbol;
        case ChordTypes::MajorTriad5thBass:
            root = getNoteName(noteNumbers[1]);
            chordSymbol = (root << " Major Triad / " << bassNote);
            return chordSymbol;
        //----major #5---(inversions always the same)-------------
        case ChordTypes::MajorSharp5Triad:
            root = getNoteName(noteNumbers[0]);
            chordSymbol = (root << " Major #5 Triad");
            return chordSymbol;
        //----major b5-------------------------------------------
        case ChordTypes::MajorFlat5Triad:
            root = getNoteName(noteNumbers[0]);
            chordSymbol = (root << " Major b5 Triad");
            return chordSymbol;
        case ChordTypes::MajorFlat5Triad3rdBass:
            root = getNoteName(noteNumbers[2]);
            chordSymbol = (root << " Major b5 Triad/" << bassNote);
            return chordSymbol;
        case ChordTypes::MajorFlat5Triad5thBass:
            root = getNoteName(noteNumbers[1]);
            chordSymbol = (root << " Major b5 Triad/ " << bassNote);
            return chordSymbol;
        //-----TRIADS MINOR---------------------------------------
        case ChordTypes::MinorTriad:
            root = getNoteName(noteNumbers[0]);
            chordSymbol = (root << " Minor triad / diminshed");
            return chordSymbol;
        case ChordTypes::MinorTriad3rdBass:
            root = getNoteName(noteNumbers[2]);
            chordSymbol = (root << " Minor triad /" << bassNote);
            return chordSymbol;
        case ChordTypes::MinorTraid5thBass:
            root = getNoteName(noteNumbers[1]);
            chordSymbol = (root << " Minor triad /" << bassNote);
            return chordSymbol;
        //-----minor b5-------------------------------------------
        case ChordTypes::MinorFlat5Triad:
            root = getNoteName(noteNumbers[0]);
            chordSymbol = (root << " Minor Flat 5 Triad");
            return chordSymbol;
        case ChordTypes::MinorFlat5Triad3rdBass:
            root = getNoteName(noteNumbers[2]);
            chordSymbol = (root << "Minor Flat 5 Triad /" << bassNote);
            return chordSymbol;
        case ChordTypes::MinorFlat5Triad5thBass:
            root = getNoteName(noteNumbers[1]);
            chordSymbol = (root << " Minor Flat 5 Triad /" << bassNote);
            return chordSymbol;
        //-----7th CHORDS MAJOR------------------------------------
        case ChordTypes::Maj7:
            root = getNoteName(noteNumbers[0]);
            chordSymbol = (root << " Maj7");
            return chordSymbol;
        case ChordTypes::Maj73rdBass:
            root = getNoteName(noteNumbers[3]);
            chordSymbol = (root << "Maj7 /" << bassNote);
            return chordSymbol;
        case ChordTypes::Maj75thBass:
            root = getNoteName(noteNumbers[2]);
            chordSymbol = (root << " Maj7 / " << bassNote);
            return chordSymbol;
        case ChordTypes::Maj77thBass:
            root = getNoteName(noteNumbers[1]);
            chordSymbol = (root << "Maj7 /" << bassNote);
            return chordSymbol;
        //------Maj7 omit5 ----------------------------------
        case ChordTypes::Cmaj7omit5:
            root = getNoteName(noteNumbers[0]);
            chordSymbol = (root << " Maj7(omit 5)");
            return chordSymbol;
        case ChordTypes::Cmaj7omit53rdBass:
            root = getNoteName(noteNumbers[2]);
            chordSymbol = (root << "Maj7(omit 5) /" << bassNote);
            return chordSymbol;
        case ChordTypes::Cmaj7omit57thBass:
            root = getNoteName(noteNumbers[1]);
            chordSymbol = (root << " Maj7(omit 5) /" << bassNote);
            return chordSymbol;
        //--------7th #5 -----------------------------
        case ChordTypes::Maj7Sharp5:
            root = getNoteName(noteNumbers[0]);
            chordSymbol = (root << " Major7 #5");
            return chordSymbol;
        case ChordTypes::Maj7Sharp53rdBass:
            root = getNoteName(noteNumbers[3]);
            chordSymbol = (root << " Major7 #5 /" << bassNote);
            return chordSymbol;
        case ChordTypes::Maj7Sharp55thBass:
            root = getNoteName(noteNumbers[2]);
            chordSymbol = (root <<" Major7 #5 / " << bassNote);
            return chordSymbol;
        case ChordTypes::Maj7Sharp57thBass:
            root = getNoteName(noteNumbers[1]);
            chordSymbol = (root << " Major7#5/ " << bassNote);
            return chordSymbol;
        //--------7th #11 -------------
        case ChordTypes::Maj7Sharp11:
            root = getNoteName(noteNumbers[0]);
            chordSymbol = (root << " Maj7#11");
            return chordSymbol;
        case ChordTypes::Maj7Sharp113rdBass:
            root = getNoteName(noteNumbers[3]);
            chordSymbol = (root <<" Maj7#11/ " << bassNote);
            return chordSymbol;
        case ChordTypes::Maj7Sharp115thBass:
            root = getNoteName(noteNumbers[2]);
            chordSymbol = (root << " Maj7#11/ " << bassNote);
            return chordSymbol;
        case ChordTypes::Maj7Sharp117thBass:
            root = getNoteName(noteNumbers[1]);
            chordSymbol = (root <<" Maj7#11/ " << bassNote);
            return chordSymbol;
        //--------Dominant 7 -------------
        case ChordTypes::Dominant7:
            root = getNoteName(noteNumbers[0]);
            chordSymbol = (root << " Dominant 7");
            return chordSymbol;
        case ChordTypes::Dominant73rdBass:
            root = getNoteName(noteNumbers[3]);
            chordSymbol = (root << " Dominant 7/ " << bassNote);
            return chordSymbol;
        case ChordTypes::Dominant75thBass:
            root = getNoteName(noteNumbers[2]);
            chordSymbol = (root << "Dominant 7/ " << bassNote);
            return chordSymbol;
        case ChordTypes::Dominant77thBass:
            root = getNoteName(noteNumbers[1]);
            chordSymbol = (root << " Dominant 7/ " << bassNote);
            return chordSymbol;
        //--------Dominant 7 b5 -------------
        case ChordTypes::Dominant7Flat5:
            root = getNoteName(noteNumbers[0]);
            chordSymbol = (root << " Dominant 7 b5");
            return chordSymbol;
        case ChordTypes::Dominant7Flat53rdBass:
            root = getNoteName(noteNumbers[3]);
            chordSymbol = (root <<" Dominant 7 b5/ " << bassNote);
            return chordSymbol;
        case ChordTypes::Dominant7Flat55thBass:
            root = getNoteName(noteNumbers[2]);
            chordSymbol = (root << " Dominant 7 b5/ " << bassNote);
            return chordSymbol;
        case ChordTypes::Dominant7Flat57thBass:
            root = getNoteName(noteNumbers[1]);
            chordSymbol = (root <<" Dominant 7 b5/ " << bassNote);
            return chordSymbol;
        //--------Dominant 7 b13 -------------
        case ChordTypes::Dominant7Flat13:
            root = getNoteName(noteNumbers[0]);
            chordSymbol = (root << " Dominant 7 b13");
            return chordSymbol;
        case ChordTypes::Dominant7Flat133rdBass:
            root = getNoteName(noteNumbers[3]);
            chordSymbol = (root <<" Dominant 7 b13/ " << bassNote);
            return chordSymbol;
        case ChordTypes::Dominant7Flat135thBass:
            root = getNoteName(noteNumbers[2]);
            chordSymbol = (root <<" Dominant 7 b13/ " << bassNote);
            return chordSymbol;
        case ChordTypes::Dominant7Flat137thBass:
            root = getNoteName(noteNumbers[1]);
            chordSymbol = (root <<" Dominant 7 b13/ " << bassNote);
            return chordSymbol;
        //-----7th CHORDS MINOR-----
        case ChordTypes::Minor7:
            root = getNoteName(noteNumbers[0]);
            chordSymbol = (root << "Minor 7");
            return chordSymbol;
        case ChordTypes::Minor73rdBass:
            root = getNoteName(noteNumbers[3]);
            chordSymbol = (root << "Minor 7/ " << bassNote);
            return chordSymbol;
        case ChordTypes::Minor75thBass:
            root = getNoteName(noteNumbers[2]);
            chordSymbol = (root << "Minor 7/ " << bassNote);
            return chordSymbol;
        case ChordTypes::Minor77thBass:
            root = getNoteName(noteNumbers[1]);
            chordSymbol = (root <<" Minor 7/ " << bassNote);
            return chordSymbol;
            //------minor 7 b5 --------
        case ChordTypes::Minor7Flat5:
            root = getNoteName(noteNumbers[0]);
            chordSymbol = (root <<" Minor 7 b5/ " << bassNote);
            return chordSymbol;
        case ChordTypes::Minor7Flat53rdBass:
            root = getNoteName(noteNumbers[3]);
            chordSymbol = (root <<" Minor 7 b5/ " << bassNote);
            return chordSymbol;
        case ChordTypes::Minor7Flat55thBass:
            root = getNoteName(noteNumbers[2]);
            chordSymbol = (root <<" Minor 7 b5/ " << bassNote);
            return chordSymbol;
        case ChordTypes::Minor7Flat57thBass:
            root = getNoteName(noteNumbers[1]);
            chordSymbol = (root <<" Minor 7 b5/ " << bassNote);
            return chordSymbol;

    }
}


String PRESequence::getChordSymbolAlt(std::vector<int> oneChord) { // copied get Chord symbol
//        DBG("====================================");
//        int mod;
        
//        noteNumbers.clear();
        intervals.clear();
    //    noteNumbers.clear();

        
        
        
        
        // intervals vector stores unique intervals
    //    std::vector<int> intervals;
    //    template <typename T>
    //    bool contains()
        if (oneChord.size() < 3) // less than 3 notes
        {
            DBG("Less than 3 notes");
            return String("Less than 3 notes");
        }
        

//        for (auto nm : events) { // place note names in noteNumber vector
//            int note = nm.getNote();
//            mod = note;
//            noteNumbers.emplace_back (note);
//
//        }
        
        std::sort (oneChord.begin(), oneChord.end()); // sort oneChord

        int bassNoteInt = oneChord[0]; // lowest note is the bassnote
        bassNote = getNoteName(bassNoteInt);
        
        for (auto i : oneChord) {
            int difference = (i - bassNoteInt) % 12; // get the interval difference from the lowest note and apply modolus

            if (difference != 0 && std::find (oneChord.begin(), oneChord.end(), difference) == oneChord.end())
            {

                intervals.emplace_back (difference);
//                DBG("Note added: " << difference);
            }
        }
        
        
    //    getChordMatch

        auto _intervals = chordNumbMatch.find(intervals);

        
    //    for (int i = 0; intervals.size(); ++i) {
    //        DBG("Going through for loop");
    //        DBG("intervals is: " << i);
    //    }
        



        
        
    //    for (auto nm : events) { // get note, mod and remove duplicates.
    //        int note = nm.getNote();
    //        note = note % 12;
    //        noteNumbers.push_back(note);
    ////        root = note
    ////        DBG(nm.getNote());
    //
    ////        mod = note % 12; // modulus getting only 1-12 values.
    //
    ////        if (std::find (noteNumbers.begin(), noteNumbers.end(), mod) == noteNumbers.end()) // mod != 0 &&  at start?
    ////        {
    ////            noteNumbers.emplace_back (mod);
    ////        }
    //
    ////        DBG("Modulo number: " << mod);
    //    }
        
//        DBG("oneChord[0]: " << oneChord[0]);
//        DBG("oneChord[1]: " << oneChord[1]);
//        DBG("oneChord[2]: " << oneChord[2]);
//        DBG("oneChord[3]: " << oneChord[3]);
//        DBG("oneChord[4]: " << oneChord[4]);
        
        
        if (_intervals == chordNumbMatch.end())
        {
            DBG("Cannot recognize this chord yet. DENNE");
            return String("Cannot recognize this chord yet. ");
        }

        String root;
        String chordSymbol;
        

        
        
        switch (_intervals->second)
        {
            //-----TRIADS MAJOR---------------------------------------
            case ChordTypes::MajorTriad:
                root = getNoteName(oneChord[0]);
//                chordSymbol = (root + " Major Triad");
                chordSymbol = ("| " + root + ":maj ");
//                DBG(chordSymbol);
//                chordSymbol.append(chordSymbol, 50);
                return chordSymbol;
            case ChordTypes::MajorTriad3rdBass:
                root = getNoteName(oneChord[2]);
//                chordSymbol = (root + " Major Triad / " + bassNote);
                chordSymbol = ("| " + root + ":maj/3 ");
//                chordProg.append(chordSymbol, 50);
                return chordSymbol;
            case ChordTypes::MajorTriad5thBass:
                root = getNoteName(oneChord[1]);
//                chordSymbol = (root << " Major Triad / " << bassNote);
                chordSymbol = ("| " + root + ":maj/5 ");
//                chordProg.append(chordSymbol, 50);
                return chordSymbol;
            //----major #5---(inversions always the same)-------------
            case ChordTypes::MajorSharp5Triad:
                root = getNoteName(oneChord[0]);
//                chordSym bol = (root << " Major #5 Triad");
//                chordSymbol = (root << " Major #5 Triad");
                chordSymbol = ("| " + root + ":maj ");
//                chordProg.append(chordSymbol, 50);
                return chordSymbol;
            //----major b5-------------------------------------------
            case ChordTypes::MajorFlat5Triad:
                root = getNoteName(oneChord[0]);
                chordSymbol = ("| " + root + ":maj ");
                return chordSymbol;
            case ChordTypes::MajorFlat5Triad3rdBass:
                root = getNoteName(oneChord[2]);
//                chordSymbol = (root << " Major b5 Triad/" << bassNote);
                chordSymbol = ("| " + root + ":maj/3 ");
                return chordSymbol;
            case ChordTypes::MajorFlat5Triad5thBass:
                root = getNoteName(oneChord[1]);
//                chordSymbol = (root << " Major b5 Triad/ " << bassNote);
                chordSymbol = ("| " + root + ":maj/5 ");
                return chordSymbol;
            //-----TRIADS MINOR---------------------------------------
            case ChordTypes::MinorTriad:
                root = getNoteName(oneChord[0]);
//                chordSymbol = (root << " Minor triad / diminshed");
                chordSymbol = ("| " + root + ":min ");
                return chordSymbol;
            case ChordTypes::MinorTriad3rdBass:
                root = getNoteName(oneChord[2]);
//                chordSymbol = (root << " Minor triad /" << bassNote);
                chordSymbol = ("| " + root + ":min/3 ");
                return chordSymbol;
            case ChordTypes::MinorTraid5thBass:
                root = getNoteName(oneChord[1]);
//                chordSymbol = (root << " Minor triad /" << bassNote);
                chordSymbol = ("| " + root + ":min/5 ");
                return chordSymbol;
            //-----minor b5-------------------------------------------
            case ChordTypes::MinorFlat5Triad:
                root = getNoteName(oneChord[0]);
//                chordSymbol = (root << " Minor Flat 5 Triad");
                chordSymbol = ("| " + root + ":min ");
                return chordSymbol;
            case ChordTypes::MinorFlat5Triad3rdBass:
                root = getNoteName(oneChord[2]);
//                chordSymbol = (root << "Minor Flat 5 Triad /" << bassNote);
                chordSymbol = ("| " + root + ":min/3 ");
                return chordSymbol;
            case ChordTypes::MinorFlat5Triad5thBass:
                root = getNoteName(oneChord[1]);
//                chordSymbol = (root << " Minor Flat 5 Triad /" << bassNote);
                chordSymbol = ("| " + root + ":min/5 ");
                return chordSymbol;
            //-----7th CHORDS MAJOR------------------------------------
            case ChordTypes::Maj7:
                root = getNoteName(oneChord[0]);
//                chordSymbol = (root << " Maj7");
                chordSymbol = ("| " + root + ":maj7 ");
                return chordSymbol;
            case ChordTypes::Maj73rdBass:
                root = getNoteName(oneChord[3]);
//                chordSymbol = (root << "Maj7 /" << bassNote);
                chordSymbol = ("| " + root + ":maj7/3 ");
                return chordSymbol;
            case ChordTypes::Maj75thBass:
                root = getNoteName(oneChord[2]);
//                chordSymbol = (root << " Maj7 / " << bassNote);
                chordSymbol = ("| " + root + ":maj7/5 ");
                return chordSymbol;
            case ChordTypes::Maj77thBass:
                root = getNoteName(oneChord[1]);
//                chordSymbol = (root << "Maj7 /" << bassNote);
                chordSymbol = ("| " + root + ":maj7/7 ");
                return chordSymbol;
            //------Maj7 omit5 ----------------------------------
            case ChordTypes::Cmaj7omit5:
                root = getNoteName(oneChord[0]);
//                chordSymbol = (root << " Maj7(omit 5)");
                chordSymbol = ("| " + root + ":maj7 ");
                return chordSymbol;
            case ChordTypes::Cmaj7omit53rdBass:
                root = getNoteName(oneChord[2]);
//                chordSymbol = (root << "Maj7(omit 5) /" << bassNote);
                chordSymbol = ("| " + root + ":maj7/5 ");
                return chordSymbol;
            case ChordTypes::Cmaj7omit57thBass:
                root = getNoteName(oneChord[1]);
//                chordSymbol = (root << " Maj7(omit 5) /" << bassNote);
                chordSymbol = ("| " + root + ":maj7/7 ");
                return chordSymbol;
            //--------7th #5 -----------------------------
            case ChordTypes::Maj7Sharp5:
                root = getNoteName(oneChord[0]);
//                chordSymbol = (root << " Major7 #5");
                chordSymbol = ("| " + root + ":maj7 ");
                return chordSymbol;
            case ChordTypes::Maj7Sharp53rdBass:
                root = getNoteName(oneChord[3]);
//                chordSymbol = (root << " Major7 #5 /" << bassNote);
                chordSymbol = ("| " + root + ":maj7/3 ");
                return chordSymbol;
            case ChordTypes::Maj7Sharp55thBass:
                root = getNoteName(oneChord[2]);
//                chordSymbol = (root <<" Major7 #5 / " << bassNote);
                chordSymbol = ("| " + root + ":maj7/5 ");
                return chordSymbol;
            case ChordTypes::Maj7Sharp57thBass:
                root = getNoteName(oneChord[1]);
//                chordSymbol = (root << " Major7#5/ " << bassNote);
                chordSymbol = ("| " + root + ":maj7/7 ");
                return chordSymbol;
            //--------7th #11 -------------
            case ChordTypes::Maj7Sharp11:
                root = getNoteName(oneChord[0]);
                chordSymbol = (root << " Maj7#11 ");
                return chordSymbol;
            case ChordTypes::Maj7Sharp113rdBass:
                root = getNoteName(oneChord[3]);
                chordSymbol = (root <<" Maj7#11/ " << bassNote);
                return chordSymbol;
            case ChordTypes::Maj7Sharp115thBass:
                root = getNoteName(oneChord[2]);
                chordSymbol = (root << " Maj7#11/ " << bassNote);
                return chordSymbol;
            case ChordTypes::Maj7Sharp117thBass:
                root = getNoteName(oneChord[1]);
                chordSymbol = (root <<" Maj7#11/ " << bassNote);
                return chordSymbol;
            //--------Dominant 7 -------------
            case ChordTypes::Dominant7:
                root = getNoteName(oneChord[0]);
//                chordSymbol = (root << " Dominant 7");
                chordSymbol = ("| " + root + ":7 ");
                return chordSymbol;
            case ChordTypes::Dominant73rdBass:
                root = getNoteName(oneChord[3]);
//                chordSymbol = (root << " Dominant 7/ " << bassNote);
                chordSymbol = ("| " + root + ":7/3 ");
                return chordSymbol;
            case ChordTypes::Dominant75thBass:
                root = getNoteName(oneChord[2]);
//                chordSymbol = (root << "Dominant 7/ " << bassNote);
                chordSymbol = ("| " + root + ":7/5 ");
                return chordSymbol;
            case ChordTypes::Dominant77thBass:
                root = getNoteName(oneChord[1]);
//                chordSymbol = (root << " Dominant 7/ " << bassNote);
                chordSymbol = ("| " + root + ":7/7 ");
                return chordSymbol;
            //--------Dominant 7 b5 -------------
            case ChordTypes::Dominant7Flat5:
                root = getNoteName(oneChord[0]);
                chordSymbol = (root << " Dominant 7 b5 ");
                return chordSymbol;
            case ChordTypes::Dominant7Flat53rdBass:
                root = getNoteName(oneChord[3]);
                chordSymbol = (root <<" Dominant 7 b5/ " << bassNote);
                return chordSymbol;
            case ChordTypes::Dominant7Flat55thBass:
                root = getNoteName(oneChord[2]);
                chordSymbol = (root << " Dominant 7 b5/ " << bassNote);
                return chordSymbol;
            case ChordTypes::Dominant7Flat57thBass:
                root = getNoteName(oneChord[1]);
                chordSymbol = (root <<" Dominant 7 b5/ " << bassNote);
                return chordSymbol;
            //--------Dominant 7 b13 -------------
            case ChordTypes::Dominant7Flat13:
                root = getNoteName(oneChord[0]);
                chordSymbol = (root << " Dominant 7 b13 ");
                return chordSymbol;
            case ChordTypes::Dominant7Flat133rdBass:
                root = getNoteName(oneChord[3]);
                chordSymbol = (root <<" Dominant 7 b13/ " << bassNote);
                return chordSymbol;
            case ChordTypes::Dominant7Flat135thBass:
                root = getNoteName(oneChord[2]);
                chordSymbol = (root <<" Dominant 7 b13/ " << bassNote);
                return chordSymbol;
            case ChordTypes::Dominant7Flat137thBass:
                root = getNoteName(oneChord[1]);
                chordSymbol = (root <<" Dominant 7 b13/ " << bassNote);
                return chordSymbol;
            //-----7th CHORDS MINOR-----
            case ChordTypes::Minor7:
                root = getNoteName(oneChord[0]);
//                chordSymbol = (root << "Minor 7");
                chordSymbol = ("| " + root + ":min7 ");
                return chordSymbol;
            case ChordTypes::Minor73rdBass:
                root = getNoteName(oneChord[3]);
//                chordSymbol = (root << "Minor 7/ " << bassNote);
                chordSymbol = ("| " + root + ":min7/3 ");
                return chordSymbol;
            case ChordTypes::Minor75thBass:
                root = getNoteName(oneChord[2]);
//                chordSymbol = (root << "Minor 7/ " << bassNote);
                chordSymbol = ("| " + root + ":min7/5 ");
                return chordSymbol;
            case ChordTypes::Minor77thBass:
                root = getNoteName(oneChord[1]);
//                chordSymbol = (root <<" Minor 7/ " << bassNote);
                chordSymbol = ("| " + root + ":min7/7 ");
                return chordSymbol;
                //------minor 7 b5 --------
            case ChordTypes::Minor7Flat5:
                root = getNoteName(oneChord[0]);
                chordSymbol = (root <<" Minor 7 b5/ " << bassNote);
                return chordSymbol;
            case ChordTypes::Minor7Flat53rdBass:
                root = getNoteName(oneChord[3]);
                chordSymbol = (root <<" Minor 7 b5/ " << bassNote);
                return chordSymbol;
            case ChordTypes::Minor7Flat55thBass:
                root = getNoteName(oneChord[2]);
                chordSymbol = (root <<" Minor 7 b5/ " << bassNote);
                return chordSymbol;
            case ChordTypes::Minor7Flat57thBass:
                root = getNoteName(oneChord[1]);
                chordSymbol = (root <<" Minor 7 b5/ " << bassNote);
                return chordSymbol;

        }
    
}

//void readFile (const juce::File& fileToRead)
//{
//    if (! fileToRead.existsAsFile()) // [1]
//        return;
//
////        DBG(String(fileToRead));
//
//    juce::FileInputStream inputStream (fileToRead); // [2]
//
//    if (! inputStream.openedOk())
//        return;  // failed to open
//
////    textContent->clear();
//
////    auto normalFont = textContent->getFont();
////    auto titleFont  = normalFont.withHeight (normalFont.getHeight() * 1.5f).boldened();
//    juce::String asterix ("|");
//    juce::String tonic ("# tonic:");
//    juce::String comma (",");
//
//
////               first part of tutorial
//    while (! inputStream.isExhausted()) // [3]
//    {
//        auto line = inputStream.readNextLine();
//
//
////            auto key = line.fromFirstOccurrenceOf(tonic, false, false);
////            DBG(key);
//
//        if (line.startsWith(tonic)) {
//            auto key = line.fromFirstOccurrenceOf(tonic, false, false);
//            DBG(key);
//        }
//
//        line = line.fromFirstOccurrenceOf(asterix, true, false);
//
//        line = line.upToLastOccurrenceOf(asterix, true, false);
//
//        juce::String chords;
//        chords.append(line, 1000);
//        DBG(chords);
//
//
//
////            auto rem = line.fromLastOccurrenceOf(asterix, false, false);
////            line.fromLastOccurrenceOf
////            DBG(rem);
////            auto first = line.indexOfAnyOf(rem);
////            DBG(first);
//
////            if (line.contains()
//
////            line = line.removeCharacters(rem);
////            if (line.fromLastOccurrenceOf(asterix, false, false)) {
////                continue;
////            }
//
//
////            if (line.startsWith (asterix))
////            {
////                line = line.removeCharacters (asterix);
////            textContent->setFont (titleFont);
////            }
////            else
////            {
////            textContent->setFont (normalFont);
////            }
//
//        // append the text to the textContent
////        textContent->insertTextAtCaret (line + juce::newLine);
//
//
////            second part of tutorial
////        while (! inputStream.isExhausted())
////        {
////            auto nextWord = readUpToNextSpace (inputStream);
////            textContent->setColour (juce::TextEditor::textColourId, Colours::red);
////            textContent->insertTextAtCaret (nextWord);
//    }
//
//
////        searchSimilarSongs();
//}
//
//
//void searchChordLib() {
//    std::vector<File> chordLib;
//
////        DBG("hei");
//
//    for (DirectoryEntry entry : RangedDirectoryIterator (File ("/Users/axelborgmo/NYU/Thesis/Harmoni/Harmoni/MIDI-Library/The-McGill-Billboard-Project/Complete-Annotations/McGill-Billboard"), true)) {
////            File.getChildFile("salami_chords copy.txt");
//        chordLib.push_back (entry.getFile());
////                DBG("hei");
////            readFile(());
//    }
//
//    readFile(chordLib[0]);
//
//}


String PRESequence::searchSimilarSongs() {

    
    std::vector <int> _oneChord;
    String chordProg;
    int block = 480;
    int numNotes = 1;
    

    
    for (auto nm : events) { // place note names in noteNumber vector
//        DBG("===========");
        int note = nm.getNote();
        int quart = nm.getStartTime();

        
        if (quart < block) {
            _oneChord.emplace_back(note);
        }
        
        else if (quart >= block) {
            if (numNotes > 3) {
                String chord = getChordSymbolAlt(_oneChord);
                chordProg.append(chord, 50);
                block += 480;
                _oneChord.clear();
                _oneChord.emplace_back(note);
            }
            else { // if first chord start after first quarternote
                _oneChord.emplace_back(note);
                chordProg = ("| |");
                block += 480;
            }
        }
        numNotes += 1;
    
    }
    String chord = getChordSymbolAlt(_oneChord);
    chordProg.append(chord, 50);
    
//    if (block <= 480 && numNotes >= 3) {    //    if all notes have been added and they are all within first quarternote:
//        DBG("block is less or equal to 480 and we call getChordSymbolAlt. Block is: " << block);
//        String chord = getChordSymbolAlt(_oneChord);
//        chordProg.append(chord, 50);
//    }
    
//    searchChordLib();
    
    return chordProg;
    
}





