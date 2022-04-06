/*
  ==============================================================================

    NoteMessage.h
    Created: 2 Feb 2022 3:57:45pm
    Modified from Samuel Hunt: https://github.com/Sjhunt93/Piano-Roll-Editor

  ==============================================================================
*/

#pragma once

// not sure if needed #include <JuceHeader.h>
#include "PConstants.h"
#include <vector>
#include <iostream>
#include <list>
#include <unordered_map>
#include <algorithm>

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
        
        unsigned int state : 2; // 2 bits long
        unsigned int isGenerative : 2; // 2 bits long
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
    
    std::function<void(int note, int velocity)> sendChange;
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



enum class ChordTypes : char
{
    //-----TRIADS MAJOR-----
    MajorTriad,
    MajorTriad3rdBass,
    MajorTriad5thBass,
    //---major #5---(inversions always the same)
    MajorSharp5Triad,
    //---major b5---------
    MajorFlat5Triad,
    MajorFlat5Triad3rdBass,
    MajorFlat5Triad5thBass,
    //-----TRIADS MINOR-----
    MinorTriad,
    MinorTriad3rdBass,
    MinorTraid5thBass,
    //-----minor b5------
    MinorFlat5Triad,
    MinorFlat5Triad3rdBass,
    MinorFlat5Triad5thBass,
    //-----7th CHORDS MAJOR-----
    Maj7,
    Maj73rdBass,
    Maj75thBass,
    Maj77thBass,
    //------Maj7 omit5 ----
    Cmaj7omit5,
    Cmaj7omit53rdBass,
    Cmaj7omit57thBass,
    //--------7th #5 -------------
    Maj7Sharp5,
    Maj7Sharp53rdBass,
    Maj7Sharp55thBass,
    Maj7Sharp57thBass,
    //--------7th #11 -------------
    Maj7Sharp11,
    Maj7Sharp113rdBass,
    Maj7Sharp115thBass,
    Maj7Sharp117thBass,
    //--------Dominant 7 -------------
    Dominant7,
    Dominant73rdBass,
    Dominant75thBass,
    Dominant77thBass,
    //--------Dominant 7 b5 -------------
    Dominant7Flat5,
    Dominant7Flat53rdBass,
    Dominant7Flat55thBass,
    Dominant7Flat57thBass,
    //--------Dominant 7 b13 -------------
    Dominant7Flat13,
    Dominant7Flat133rdBass,
    Dominant7Flat135thBass,
    Dominant7Flat137thBass,
    //---------Major 9th chords------------

    //-----7th CHORDS MINOR-----
    Minor7,
    Minor73rdBass,
    Minor75thBass,
    Minor77thBass,
    //------minor 7 b5 --------
    Minor7Flat5,
    Minor7Flat53rdBass,
    Minor7Flat55thBass,
    Minor7Flat57thBass
};



//hash function taken from https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
struct VectorHasher {
    std::size_t operator() (const std::vector<int> &v) const
    {
        std::size_t seed = v.size();
        for (auto& i : v)
        {
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};


const std::unordered_map<std::vector<int>, ChordTypes, VectorHasher> chordNumbMatch =
{
    //-----TRIADS MAJOR-----
    {std::vector<int> {4, 7}, ChordTypes::MajorTriad},
    {std::vector<int> {3, 8}, ChordTypes::MajorTriad3rdBass},
    {std::vector<int> {5, 9}, ChordTypes::MajorTriad5thBass},
    //---major #5---(inversions always the same)
    {std::vector<int> {4, 8}, ChordTypes::MajorSharp5Triad},
    //------major b5----------
    {std::vector<int> {4, 6}, ChordTypes::MajorFlat5Triad},
    {std::vector<int> {2, 8}, ChordTypes::MajorFlat5Triad3rdBass},
    {std::vector<int> {6, 10}, ChordTypes::MajorFlat5Triad5thBass},
    //-----TRIADS MINOR-----
    {std::vector<int> {3, 7}, ChordTypes::MinorTriad},
    {std::vector<int> {4, 9}, ChordTypes::MinorTriad3rdBass,},
    {std::vector<int> {5, 8}, ChordTypes::MinorTraid5thBass,},
    //-----minor b5------
    {std::vector<int> {3, 6}, ChordTypes::MinorFlat5Triad,},
    {std::vector<int> {3, 9}, ChordTypes::MinorFlat5Triad3rdBass,},
    {std::vector<int> {6, 9}, ChordTypes::MinorFlat5Triad5thBass,},
    //-----7th CHORDS MAJOR-----
    {std::vector<int> {4, 7, 11}, ChordTypes::Maj7},
    {std::vector<int> {3, 7, 8}, ChordTypes::Maj73rdBass},
    {std::vector<int> {4, 5, 9}, ChordTypes::Maj75thBass},
    {std::vector<int> {1, 5, 8}, ChordTypes::Maj77thBass},
    //------Maj7 omit5 ----
    {std::vector<int> {4, 11}, ChordTypes::Cmaj7omit5},
    {std::vector<int> {6, 7}, ChordTypes::Cmaj7omit53rdBass},
    {std::vector<int> {1, 5}, ChordTypes::Cmaj7omit57thBass},
    //--------7th #5 -------------
    {std::vector<int> {4, 8, 11}, ChordTypes::Maj7Sharp5},
    {std::vector<int> {4, 7, 8}, ChordTypes::Maj7Sharp53rdBass},
    {std::vector<int> {3, 4, 8}, ChordTypes::Maj7Sharp55thBass},
    {std::vector<int> {1, 5, 9}, ChordTypes::Maj7Sharp57thBass},
    //--------7th #11 -------------
    {std::vector<int> {4, 6, 11}, ChordTypes::Maj7Sharp11},
    {std::vector<int> {2, 7, 8}, ChordTypes::Maj7Sharp113rdBass},
    {std::vector<int> {5, 6, 10}, ChordTypes::Maj7Sharp115thBass},
    {std::vector<int> {1, 5, 7}, ChordTypes::Maj7Sharp117thBass},
    //--------Dominant 7 -------------
    {std::vector<int> {4, 7, 10}, ChordTypes::Dominant7},
    {std::vector<int> {3, 6, 8}, ChordTypes::Dominant73rdBass},
    {std::vector<int> {3, 5, 9}, ChordTypes::Dominant75thBass},
    {std::vector<int> {3, 5, 9}, ChordTypes::Dominant77thBass},
    //--------Dominant 7 b5 -------------
    {std::vector<int> {4, 6, 10}, ChordTypes::Dominant7Flat5},
    {std::vector<int> {2, 6, 8}, ChordTypes::Dominant7Flat53rdBass},
    {std::vector<int> {4, 6, 10}, ChordTypes::Dominant7Flat55thBass},
    {std::vector<int> {2, 6, 9}, ChordTypes::Dominant7Flat57thBass},
    //--------Dominant 7 b13 -------------
    {std::vector<int> {3, 8, 8}, ChordTypes::Dominant7Flat13},
    {std::vector<int> {4, 6, 8}, ChordTypes::Dominant7Flat133rdBass},
    {std::vector<int> {2, 4, 8}, ChordTypes::Dominant7Flat135thBass},
    {std::vector<int> {2, 6, 10}, ChordTypes::Dominant7Flat137thBass},
    //-----7th CHORDS MINOR-----
    {std::vector<int> {3, 7, 10}, ChordTypes::Minor7},
    {std::vector<int> {4, 7, 9}, ChordTypes::Minor73rdBass},
    {std::vector<int> {3, 5, 8}, ChordTypes::Minor75thBass},
    {std::vector<int> {2, 5, 9}, ChordTypes::Minor77thBass},
    //------minor 7 b5 --------
    {std::vector<int> {3, 6, 10}, ChordTypes::Minor7Flat5},
    {std::vector<int> {3, 7, 9}, ChordTypes::Minor7Flat53rdBass},
    {std::vector<int> {3, 5, 9}, ChordTypes::Minor7Flat55thBass},
    {std::vector<int> {2, 5, 8}, ChordTypes::Minor7Flat57thBass},
    
    
    
};





class PRESequence
{
public:
    PRESequence() {}

    ~PRESequence() {}
    std::vector<NoteMessage> events;
    std::vector<int> intervals;
    std::vector<int> noteNumbers;
    std::vector<int> quarter;
    int tsLow;
    int tsHight;
    
    int lowNote;
    int highNote;
    String bassNote;
        

    File exportMIDI();
    String getNoteName(int intNote);
    
    String getChordSymbol();
    String getChordSymbolAlt(std::vector<int> oneChord);
//    String searchChordLib();
//    String readFile(const juce::File& fileToRead);
    String searchSimilarSongs();
    
private:
//    chordLibrary library;
    
//    String findChordMatch();

    
};

    
    
