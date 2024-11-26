// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0

#ifndef DRONE_H
#define DRONE_H

#include <string>
#include <getopt.h> // assuming you need getopt.h here
#include <vector>

// Enum to represent stack or queue modes
enum class whichMode {
    kNone,
    kMST,
    kFASTTSP,
    kOPTTSP
};

enum class whichCampus {
    kNone,
    kMedical,
    kBorder,
    kMain
};

// Struct to hold options
struct Options {
    whichMode whichModeIsIt = whichMode::kNone;
};

struct Vertex {
    int x;
    int y;
    whichCampus campusType; // Member variable to store campus type
    size_t vertexIdx; // Member variable to store vertex index

    // Constructor to initialize x, y, and vertexIdx, and set campusType to kNone
    Vertex(int x_, int y_, size_t vertexIdx_)
        : x(x_), y(y_), campusType(whichCampus::kNone), vertexIdx(vertexIdx_) {}
};

class Drone {
public:
    // four vectors - k v d p

    bool med_exists = false;
    bool main_exists = false;
    bool border_exists = false;

    int mapSize = 0;

    std::vector<Vertex> vertices;

    // Function to print help message
    static void printHelp(char *argv[]);

    // Function to parse command line options
    static void getOptions(int argc, char * argv[], Options &options);

    void readInput(Options &options);
};

#endif // DRONE_H