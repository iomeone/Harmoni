/*
  ==============================================================================

    ChordLibrary.h
    Created: 27 Mar 2022 5:21:19pm
    Author:  Axel Borgmo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <algorithm>
#include <map>

using namespace juce;

class ChordLibrary   :  public juce::Component,
                        public juce::FilenameComponentListener
{
public:
    ChordLibrary()
    {
        
        
        
//        fileComp.reset (new juce::FilenameComponent ("fileComp",
//                                                     {},                       // current file
//                                                     false,                    // can edit file name,
//                                                     false,                    // is directory,
//                                                     false,                    // is for saving,
//                                                     {},                       // browser wildcard suffix,
//                                                     {},                       // enforced suffix,
//                                                     "Select file to open"));  // text when nothing selected
//        addAndMakeVisible (fileComp.get());
//        fileComp->addListener (this);
//        loadChords.onClick = [this] { searchSimilarSongs(); };
//
//        addAndMakeVisible(loadChords);
////        loadChords.set
//
        textContent.reset (new juce::TextEditor());
        addAndMakeVisible (textContent.get());
        textContent->setMultiLine (true);
        textContent->setReadOnly (true);
        textContent->setCaretVisible (false);
        textContent->setPopupMenuEnabled(true);
        textContent->setLineSpacing(1.5);
 
        setSize (600, 400);
    }

    void resized() override
    {
//        fileComp->setBounds    (10, 10, getWidth() - 20, 20);
//        loadChords.setBounds(10,10,getWidth() - 20, 20);
        textContent->setBounds (10, 40, getWidth() - 20, getHeight() - 50);

    }
    
    
    int levenshtein(std::string chords1, std::string chords2) {
        using namespace std;
        int m = chords1.size();
        int n = chords2.size();
        
        int ** dp = new int * [chords1.size() + 1];
        
        for (int i = 0; i < chords1.size() + 1; i++) {
            dp[i] = new int[chords2.size() + 1];
        }
        
        dp[0][0] = 0;
        
        for (int i = 1; i < m + 1; i++){
            dp[i][0] = i;
        }
        for (int i = 1; i < n + 1; i++){
            dp[0][i] = i;
        }
        
        for (int i = 1; i < m + 1; i++){
            
            for (int j = 1; j < n + 1; j++){
                if (chords1[m - i] == chords2[n - j]){
                    dp[i][j] = dp[i - 1][j - 1];
                }
                
                else {
                    dp[i][j] = 1 + min(dp[i][j - 1], min(dp[i - 1][j], dp[i -1][j - 1]));
                }
            }
        }
        
        return dp[m][n];
    }
    
    

    void filenameComponentChanged (juce::FilenameComponent* fileComponentThatHasChanged) override
    {
        if (fileComponentThatHasChanged == fileComp.get())
            readFile (fileComp->getCurrentFile());
    }
    

    
    
    String transposeChordLib(String keySig, String _chords) {
        String C = "C";
        String Db = "Db";
        String D = "D";
        String Eb = "Eb";
        String E = "E";
        String F = "F";
        String Gb = "Gb";
        String G = "G";
        String Ab = "Ab";
        String A = "A";
        String Bb = "Bb";
        String B = "B";
        
//        DBG("keysig is: " << keySig);
        
        if (keySig == "C") {
//            DBG("Do nothing");
        }
        
        else if (keySig == "Db") {
//            DBG("keysig == Db");
            _chords = _chords.replace("Db:", "C:", false);
//            _chords = _chords.replace("D", "Db", false);
            _chords = _chords.replace("Eb:", "D:", false);
//            _chords = _chords.replace("E", "Eb", false);
            _chords = _chords.replace("F:", "E:", false);
            _chords = _chords.replace("Gb:", "F:", false);
//            _chords = _chords.replace("G", "Gb", false);
            _chords = _chords.replace("Ab:", "G:", false);
//            _chords = _chords.replace("A", "Ab", false);
            _chords = _chords.replace("Bb:", "A:", false);
//            _chords = _chords.replace("B", "Bb", false);
//            _chords = _chords.replace("C", "B", false);
        }
            
        else if (keySig == "D") {
//            DBG("keysig == D");
            _chords = _chords.replace("D:", "C:", false);
//            _chords = _chords.replace("Eb", "Db", false);
            _chords = _chords.replace("E:", "D:", false);
//            _chords = _chords.replace("F", "Eb", false);
            _chords = _chords.replace("Gb:", "E:", false);
            _chords = _chords.replace("G:", "F:", false);
//            _chords = _chords.replace("Ab", "Gb", false);
            _chords = _chords.replace("A:", "G:", false);
//            _chords = _chords.replace("Bb", "Ab", false);
            _chords = _chords.replace("B:", "A:", false);
//            _chords = _chords.replace("C", "Bb", false);
//            _chords = _chords.replace("Db", "B", false);
            
        }
        else if (keySig == "E") {
            DBG("keysig == E");
            _chords = _chords.replace("E:", "C:", false);
//            _chords = _chords.replace("F", "Db", false);
            _chords = _chords.replace("Gb:", "D:", false);
//            _chords = _chords.replace("G", "Eb", false);
            _chords = _chords.replace("Ab:", "E:", false);
            _chords = _chords.replace("A:", "F:", false);
//            _chords = _chords.replace("Bb", "Gb", false);
            _chords = _chords.replace("B:", "G:", false);
//            _chords = _chords.replace("C", "Ab", false);
            _chords = _chords.replace("Db:", "A:", false);
//            _chords = _chords.replace("D", "Bb", false);
//            _chords = _chords.replace("Eb", "B", false);
            
        }
        else if (keySig == "F") {
//            DBG("keysig == "Eb");
            _chords = _chords.replace("F:", "C:", false);
//            _chords = _chords.replace("Gb", "Db", false);
            _chords = _chords.replace("G:", "D:", false);
//            _chords = _chords.replace("Ab", "Eb", false);
            _chords = _chords.replace("A:", "E:", false);
            _chords = _chords.replace("Bb:", "F:", false);
//            _chords = _chords.replace("B", "Gb", false);
            _chords = _chords.replace("C:", "G:", false);
//            _chords = _chords.replace("Db", "Ab", false);
            _chords = _chords.replace("D:", "A:", false);
//            _chords = _chords.replace("Eb", "Bb", false);
//            _chords = _chords.replace("E", "B", false);
        }
        else if (keySig == "Gb") {
//            DBG("keysig == "Eb");
            _chords = _chords.replace("Gb:", "C:", false);
//            _chords = _chords.replace("G", "Db", false);
            _chords = _chords.replace("Ab:", "D:", false);
//            _chords = _chords.replace("A", "Eb", false);
            _chords = _chords.replace("Bb:", "E:", false);
            _chords = _chords.replace("B:", "F:", false);
//            _chords = _chords.replace("C", "Gb", false);
            _chords = _chords.replace("Db:", "G:", false);
//            _chords = _chords.replace("D", "Ab", false);
            _chords = _chords.replace("Eb:", "A:", false);
//            _chords = _chords.replace("E", "Bb", false);
//            _chords = _chords.replace("F", "B", false);
        }
        else if (keySig == "G") {
//            DBG("keysig == "Eb");
            _chords = _chords.replace("G:", "C:", false);
//            _chords = _chords.replace("Ab", "Db", false);
            _chords = _chords.replace("A:", "D:", false);
//            _chords = _chords.replace("Bb", "Eb", false);
            _chords = _chords.replace("B:", "E:", false);
            _chords = _chords.replace("C:", "F:", false);
//            _chords = _chords.replace("B", "Gb", false);
            _chords = _chords.replace("C:", "G:", false);
//            _chords = _chords.replace("Db", "Ab", false);
            _chords = _chords.replace("D:", "A:", false);
//            _chords = _chords.replace("Eb", "Bb", false);
//            _chords = _chords.replace("E", "B", false);
        }
        else if (keySig == "Ab") {
//            DBG("keysig == "Eb");
            _chords = _chords.replace("Ab:", "C:", false);
//            _chords = _chords.replace("A", "Db", false);
            _chords = _chords.replace("Bb:", "D:", false);
//            _chords = _chords.replace("B", "Eb", false);
            _chords = _chords.replace("C:", "E:", false);
            _chords = _chords.replace("Db:", "F:", false);
//            _chords = _chords.replace("D", "Gb", false);
            _chords = _chords.replace("Eb:", "G:", false);
//            _chords = _chords.replace("E", "Ab", false);
            _chords = _chords.replace("F:", "A:", false);
//            _chords = _chords.replace("Gb", "Bb", false);
//            _chords = _chords.replace("G", "B", false);
        }
        else if (keySig == "A") {
            DBG("keysig == A");
            _chords = _chords.replace("A:", "C:", false);
//            _chords = _chords.replace("Bb", "Db", false);
            _chords = _chords.replace("B:", "D:", false);
//            _chords = _chords.replace("C", "Eb", false);
            _chords = _chords.replace("Db:", "E:", false);
            _chords = _chords.replace("D:", "F:", false);
//            _chords = _chords.replace("E", "Gb", false);
            _chords = _chords.replace("F:", "G:", false);
//            _chords = _chords.replace("Gb", "Ab", false);
            _chords = _chords.replace("G:", "A:", false);
//            _chords = _chords.replace("Ab", "Bb", false);
        }
        else if (keySig == "Ab") {
//            DBG("keysig == "Eb");
            _chords = _chords.replace("Ab:", "C:", false);
//            _chords = _chords.replace("A", "Db", false);
            _chords = _chords.replace("Bb:", "D:", false);
//            _chords = _chords.replace("B", "Eb", false);
            _chords = _chords.replace("C:", "E:", false);
            _chords = _chords.replace("Db:", "F:", false);
//            _chords = _chords.replace("D", "Gb", false);
            _chords = _chords.replace("Eb:", "G:", false);
//            _chords = _chords.replace("E", "Ab", false);
            _chords = _chords.replace("F:", "A:", false);
//            _chords = _chords.replace("Gb", "Bb", false);
//            _chords = _chords.replace("G", "B", false);
        }
        else if (keySig == "A") {
//            DBG("keysig == "Eb");
            _chords = _chords.replace("A:", "C:", false);
//            _chords = _chords.replace("Bb", "Db", false);
            _chords = _chords.replace("B:", "D:", false);
//            _chords = _chords.replace("C", "Eb", false);
            _chords = _chords.replace("Db:", "E:", false);
            _chords = _chords.replace("D:", "F:", false);
//            _chords = _chords.replace("Eb", "Gb", false);
            _chords = _chords.replace("E:", "G:", false);
//            _chords = _chords.replace("F", "Ab", false);
            _chords = _chords.replace("Gb:", "A:", false);
//            _chords = _chords.replace("G", "Bb", false);
//            _chords = _chords.replace("Ab", "B", false);
        }
        else if (keySig == "Bb") {
//            DBG("keysig == "Eb");
            _chords = _chords.replace("Bb:", "C:", false);
//            _chords = _chords.replace("B", "Db", false);
            _chords = _chords.replace("C:", "D:", false);
//            _chords = _chords.replace("Db", "Eb", false);
            _chords = _chords.replace("D:", "E:", false);
            _chords = _chords.replace("Eb:", "F:", false);
//            _chords = _chords.replace("E", "Gb", false);
            _chords = _chords.replace("F:", "G:", false);
//            _chords = _chords.replace("Gb", "Ab", false);
            _chords = _chords.replace("G:", "A:", false);
//            _chords = _chords.replace("Ab", "Bb", false);
//            _chords = _chords.replace("A", "B", false);
        }
        else if (keySig == "B") {
//            DBG("keysig == "Eb");
            _chords = _chords.replace("B:", "C:", false);
//            _chords = _chords.replace("C", "Db", false);
            _chords = _chords.replace("Db:", "D:", false);
//            _chords = _chords.replace("D", "Eb", false);
            _chords = _chords.replace("Eb:", "E:", false);
            _chords = _chords.replace("E:", "F:", false);
//            _chords = _chords.replace("F", "Gb", false);
            _chords = _chords.replace("Gb:", "G:", false);
//            _chords = _chords.replace("G", "Ab", false);
            _chords = _chords.replace("Ab:", "A:", false);
//            _chords = _chords.replace("A", "Bb", false);
//            _chords = _chords.replace("Bb", "B", false);
        }

        
    return _chords;
    }

    String readFile (const juce::File& fileToRead)
    {
        if (! fileToRead.existsAsFile()) // [1]
            return {"No file"};

//        DBG(String(fileToRead));
        
        juce::FileInputStream inputStream (fileToRead); // [2]
         
        if (! inputStream.openedOk())
            return {"Failed to open"};  // failed to open
        
//        textContent->clear();
         
//        auto normalFont = textContent->getFont();
//        auto titleFont  = normalFont.withHeight (normalFont.getHeight() * 1.5f).boldened();
        juce::String asterix ("|");
        juce::String tonic ("# tonic: ");
        
        juce::String comma (",");
        juce::String chords;
//        juce::String _chords;
        
        
//               first part of tutorial
        while (! inputStream.isExhausted()) // [3]
        {
            auto line = inputStream.readNextLine();
            
       
//            auto key = line.fromFirstOccurrenceOf(tonic, false, false);
//            DBG(key);
            
            if (line.startsWith(tonic)) {
                key = line.fromFirstOccurrenceOf(tonic, false, false);
//                DBG(key);
            }
            
            line = line.fromFirstOccurrenceOf(asterix, true, false);
            
            
            
//            line = line.upToLastOccurrenceOf(asterix, true, false);
            
//            if (line.contains)
            
            
            chords.append(line, 1000);
            
//            DBG(chords);
            
//            auto rem = line.fromLastOccurrenceOf(asterix, false, false);
//            line.fromLastOccurrenceOf
//            DBG(rem);
//            auto first = line.indexOfAnyOf(rem);
//            DBG(first);
            
//            if (line.contains()
            
//            line = line.removeCharacters(rem);
//            if (line.fromLastOccurrenceOf(asterix, false, false)) {
//                continue;
//            }
            

//            if (line.startsWith (asterix))
//            {
//                line = line.removeCharacters (asterix);
//                textContent->setFont (titleFont);
//            }
//            else
//            {
//                textContent->setFont (normalFont);
//            }

            // append the text to the textContent
//            textContent->insertTextAtCaret (line + juce::newLine);
            
            
//            second part of tutorial
//        while (! inputStream.isExhausted())
//        {
//            auto nextWord = readUpToNextSpace (inputStream);
//            textContent->setColour (juce::TextEditor::textColourId, Colours::red);
//            textContent->insertTextAtCaret (nextWord);
        }
        DBG("key is:" << key);
        
        chords = transposeChordLib(key, chords);
        DBG("Chords are" << chords);
        
        return chords;
        
        
        
        
//        searchSimilarSongs();
    }


    
    
    
    int compareChords(String pianoRollChords, String libChords) {
        // converting juce::string to std::string
        std::string _pianoRollChords = pianoRollChords.toStdString();
        std::string _libChords = libChords.toStdString();

        // find numbers of chords in piano roll
        int numChordsPi = std::count(_pianoRollChords.begin(), _pianoRollChords.end(), '|');

        
        // find index of '|' in libChords that corresponds to numChords in PianoRoll
        size_t  pos = 0;
        int     cnt = 0;

        while( cnt != numChordsPi )
        {
            pos+=1;
            pos = _libChords.find("|", pos);
            if ( pos == std::string::npos )
                break;
            cnt++;
        }
        
        // make libChords with same amount of chords as pianoRollChords
        String libSameLen = libChords.substring(0, pos);
        std::string _libSameLen = libSameLen.toStdString();
        
//        DBG("lib chords are: " << _libSameLen);
        
        
        int score = 100 - (levenshtein(_pianoRollChords, _libSameLen));
        
        return score;
        
    }
    
    
    
    

    
    static juce::Colour getRandomColour (float minBrightness)
    {
        auto& random = juce::Random::getSystemRandom();
        juce::Colour colour ((juce::uint8) random.nextInt (256),
                             (juce::uint8) random.nextInt (256),
                             (juce::uint8) random.nextInt (256));
 
        return colour.getBrightness() >= minBrightness ? colour
                                                       : colour.withBrightness (minBrightness);
    }
    
    static juce::String readUpToNextSpace (juce::FileInputStream& inputStream)
    {
        juce::MemoryBlock buffer (256);
        auto* data = static_cast<char*> (buffer.getData());
        size_t i = 0;
 
        while ((data[i] = inputStream.readByte()) != 0 && i < buffer.getSize())
            if (data[i++] == '|')
                break;

 
        return juce::String::fromUTF8 (data, (int) i); // [4]
    }
    
    
    void searchChordLibrary(String _chordProg) {
//        std::vector<File> hiddenFiles;
        String chordss;
        int scoreMax = 0, scoreTmp = 0;
        topSongs.clear();
        topScores.clear();
        
//        DBG("hei");
         
        for (DirectoryEntry entry : RangedDirectoryIterator (File ("/Users/axelborgmo/NYU/Thesis/Harmoni/Harmoni/MIDI-Library/The-McGill-Billboard-Project/Complete-Annotations/McGill-Billboard/"), true)) {
//            File.getChildFile("salami_chords copy.txt");
//                hiddenFiles.push_back (entry.getFile());
                chordss = readFile(entry.getFile());
                scoreTmp = compareChords(_chordProg, chordss);
//            DBG("hei");
                if (scoreTmp > scoreMax) {
                    scoreMax = scoreTmp;
//                    DBG("Score max is now: " << scoreMax);
                    File file = entry.getFile();
                    String path = file.getFullPathName();
                    std::string _path = path.toStdString();
                    topSongs.insert(topSongs.begin(), _path);
                    topScores.insert(topScores.begin(), scoreMax);
                    DBG("size of vector is: " << topSongs.size());
    //                topSongs.emplace_back(file.getFullPathName());
                }
                else {
                    File file = entry.getFile();
                    String path = file.getFullPathName();
                    std::string _path = path.toStdString();
                    topSongs.push_back(_path);
                    topScores.push_back(scoreTmp);
                }
//                DBG("hei");
    //            readFile(());
        }
        
//        DBG("Top song 0: " << topSongs[0]);
//        DBG("Top score 0: " << topScores[0]);
//        DBG("Top song 1: " << topSongs[1]);
//        DBG("Top score 1: " << topScores[1]);
//        DBG("Top song 2: " << topSongs[2]);
//        DBG("Top score 2: " << topScores[2]);
        
//        displayTopSongs();
        
//        DBG("Top song is:" << topSongs.begin()->first);
//        DBG("Second place is:" << topSongs[1]);
//        DBG("Third place is:" << topSongs[2]);
        
//        DBG("SCORE IS: " << score);
//        DBG("Chord Lib is: " << chordss << "\n");
        DBG("Piano Roll chords are: " << _chordProg);
        
//        for (auto i : hiddenFiles) {
//            DBG(i.getFileName());
//        }
        
//        readFile(hiddenFiles[0]);
        
//        auto element = hiddenFiles[0];
//        DBG(element.getFileName());
//        hiddenFiles.data()
        
//
        
//        for (int i = 0; i < 10; i++){
////            DBG(hiddenFiles.at(i));
//        File hei = hiddenFiles.at(0);
//            DBG(hei.getFileName());
//        }
        
//        juce::FileInputStream inputStream (hei); // [2]
//
//// failed to open
//
//        textContent->clear();
//
//        auto normalFont = textContent->getFont();
//        auto titleFont  = normalFont.withHeight (normalFont.getHeight() * 1.5f).boldened();
//        juce::String asterix ("|");
        
//               first part of tutorial
//        while (! inputStream.isExhausted()) // [3]
//        {
//            auto line = inputStream.readNextLine();
//
//            if (line.startsWith (asterix))
//            {
//                line = line.removeCharacters (asterix);
//                textContent->setFont (titleFont);
//            }
//            else
//            {
//                textContent->setFont (normalFont);
//            }
//
//            // append the text to the textContent
//            textContent->insertTextAtCaret (line + juce::newLine);
            
            
//            second part of tutorial
//        while (! inputStream.isExhausted())
//        {
//            auto nextWord = readUpToNextSpace (inputStream);
//            textContent->setColour (juce::TextEditor::textColourId, getRandomColour (0.75f));
//            textContent->insertTextAtCaret (nextWord);
//        }
        
        
        
//        return {"Halla"};
        
    }
    
    
    void displayTopSongs(int songNumber){
        
        if (!topSongs.empty()) {
            File fileToRead(topSongs[songNumber]);
//            DBG(songNumber);
            DBG(fileToRead.getFullPathName());
            
            if (! fileToRead.existsAsFile()) // [1]
                        return;
            
            auto fileChords = fileToRead.loadFileAsString();
            String chords ("Score is: ");
            chords += (topScores[songNumber]);
            chords += ("                                                                                     ");
            chords += fileChords;
            chords += ("                                                                                     ");
            chords += ("                                                                                     ");
            chords += ("                                                                                     ");
            chords += ("In C major: ");
            chords += ("                                                                                     ");
            chords += ("                                                                                     ");
            String chordsC = readFile(fileToRead);
            chords += chordsC;
            
            
    //        DBG("Top song 0: " << topSongs[0]);
    //        DBG("Top score 0: " << topScores[0]);
    //        DBG("Top song 1: " << topSongs[1]);
    //        DBG("Top score 1: " << topScores[1]);
    //        DBG("Top song 2: " << topSongs[2]);
    //        DBG("Top score 2: " << topScores[2]);
    //        DBG("Top song 2: " << topSongs[3]);
    //        DBG("Top score 2: " << topScores[3]);
            
    //        for (DirectoryEntry entry : RangedDirectoryIterator (File ("/Users/axelborgmo/NYU/Thesis/Harmoni/Harmoni/MIDI-Library/The-McGill-Billboard-Project/Complete-Annotations/McGill-Billboard/1301"), true)) {
    ////            File.getChildFile("salami_chords copy.txt");
    ////                hiddenFiles.push_back (entry.getFile());
    //                chords = entry.getFile().loadFileAsString();
    ////                chords = readFile(entry.getFile());
    //    //                topSongs.emplace_back(file.getFullPathName());
    //                }
    //                DBG("hei");
        //            readFile(());
            

    //        chords += ("Score is: ");
    //        chords += ("Score is: " << scores[0]);
    //        auto _scores = std::to_string(scores[0]);
            

     
    //        textContent->setText(_scores);
    //        textContent->setText (chords);
            textContent->setText (chords);
        }
    }
    

    

private:
    std::unique_ptr<juce::FilenameComponent> fileComp;
    std::unique_ptr<juce::TextEditor>        textContent;
    TextButton loadChords {"Load"};
    std::vector<File> hiddenFiles;
    std::vector <String> topSongs;
    std::vector<int> topScores;
    juce::String key;
//    juce::File path ("/The-McGill-Billboard-Project/Complete-Annotations/McGill-Billboard/0003/salami_chords.txt");
//    juce::File path("/The-McGill-Billboard-Project/Complete-Annotations/McGill-Billboard/0003/salami_chords.txt");

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChordLibrary)
};

