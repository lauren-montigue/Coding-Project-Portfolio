// Project Identifier: 19034C8F3B1196BF8E0C6E1C0F973D2FD550B88F

#ifndef P2_MINE_ESCAPE_H
#define P2_MINE_ESCAPE_H

#include <deque>
#include <vector>
#include <queue>
#include <string>
#include <iostream>
#include <getopt.h>
#include <sstream>

enum class anythingExtra {
    kNone = 0,
    kStatsFlag,
    kMedianFlag,
    kVerboseFlag
};

// Structure to hold options
struct Options {

std::vector <anythingExtra> anythingExtraWhat;

};

class mineEscape {
private:
    //TODO: order most efficiently
    // Private member variables

    //TODO: implement this but somehow avoid the seg fault
    //int numTilesCleared = 0;
    //int amountRubbleCleared = 0;

    struct mapEntry {
        bool beenDiscovered = false;
        bool beenInvestigated = false;
        bool beenPushedBackToTNT = false;
        int num = 0;
    };

    std::vector<std::vector<mapEntry>> map2D;    

    int rowStart;
    int colStart;
    uint32_t sideLength;

    int statsArgument = 0;

    //in constructor have three variables to set there, three variables plus stats number

    //TODO: what is the correct way to handle these options? enum class? probs not. reference P1.
    //could potentially be easier just to have them as class variables... like this.
    //bool statsFlag = false;
    //bool medianFlag = false;
    //bool verboseFlag = false;

    // Helper function to print help message
    static void printHelp(char *argv[]);
    
public:

    //all this PQ stuff should potentially be private with a setter for rubble
    //need all this info so can be accessed in comparators in PQ
    struct PQEntry {
        int rubble;
        uint32_t row;
        uint32_t col;

        // Default constructor
        PQEntry() : rubble(0), row(0), col(0) {}

        // Constructor
        PQEntry(int rb, uint32_t r, uint32_t c) : rubble(rb), row(r), col(c) {}
    };

    

    struct PQCompare {

        bool operator()(const PQEntry& lhs, const PQEntry& rhs) const {
            //idiot check: comparing with self
            if(lhs.rubble == rhs.rubble && lhs.col == rhs.col && lhs.row == rhs.row)
                return false;
        
            if (lhs.rubble < rhs.rubble)
                return false;
            else if (lhs.rubble > rhs.rubble)
                return true;
            else
            {
                if (lhs.col < rhs.col)
                    return false;
                else if (lhs.col > rhs.col)
                    return true;
                else
                    return lhs.row > rhs.row;
            }
        }

    };

    std::priority_queue<PQEntry, std::vector<PQEntry>, PQCompare> primaryPQ;
    std::priority_queue<PQEntry, std::vector<PQEntry>, PQCompare> TNTpq;

    // Function to parse command line options
    void getOptions(int argc, char * argv[], Options &options);
    void readInput(/*const Options &options*/);
    void balanceHeaps(std::priority_queue<int>& maxHeap, std::priority_queue<int, std::vector<int>, std::greater<int>>& minHeap);
    void getOut(const Options &options);

};

#endif // P2_MINE_ESCAPE_H