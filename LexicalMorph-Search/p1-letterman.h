//IDENTIFIER  = 50EB44D3F029ED934858FFFCEAC3547C68768FC9

#ifndef LETTERMAN_H
#define LETTERMAN_H

#include <string>
#include <vector>
#include <deque>

enum class stackOrQueue {
    kNone = 0,
    kIsStack,
    kIsQueue
};

enum class wordOrMorph {
    kNone = 0,
    kWordMode,
    kMorphMode
};

enum class allowedModifications {
    kNone = 0,
    kChangeAllowed,
    kSwapAllowed,
    kLengthAllowed
};

struct Options {

    std::string beginWord;
    std::string endWord;

    stackOrQueue stackOrQueueWhich = stackOrQueue::kNone;
    wordOrMorph wordOrMorphWhich = wordOrMorph::kNone;
    std::vector <allowedModifications> allowedModificationsWhich;
};


class Letterman {
private:
    struct dictionaryEntry {
        bool beenDiscovered = false;
        std::string word = "";
        int prev = -1;
    };

    std::vector<dictionaryEntry> dictEntries;

    size_t beginWordIdx;
    size_t endWordIdx;
    
    int numWordsDiscovered = 0;
    std::deque<size_t> searchContainer;
    bool reachedEndWord = false;

    bool currentlyBacktracking = false;

    bool beginWordInDictionary = false;
    bool endWordInDictionary = false;

    bool change(const size_t &currentWordIdx, const size_t &dictionaryWordIdx);
    bool swap(const size_t &currentWordIdx, const size_t &dictionaryWordIdx);
    bool insert(const size_t &currentWordIdx, const size_t &dictionaryWordIdx);
    bool length(const size_t &currentWordIdx, const size_t &dictionaryWordIdx);

    // Flags to track if at least one of --change, --length, or --swap options is provided
    bool changeFlag = false;
    bool lengthFlag = false;
    bool swapFlag = false;

public:
    Letterman();
    void printHelp(char *argv[]);
    void getOptions(int argc, char *argv[], Options &options);
    void readDictionary(const Options &options);
    void search(const Options &options);
    void output(const Options &options);
};

#endif // LETTERMAN_H