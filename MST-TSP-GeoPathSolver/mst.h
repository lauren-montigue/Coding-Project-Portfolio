// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0

#include "drone.h"

#include <vector>
#include <string>
#include <limits>

struct primsTable {
    bool beenVisited;
    int parentIdx;
    double distance;

    // Default constructor 
    primsTable() : beenVisited(false), parentIdx(-1), distance(std::numeric_limits<double>::infinity()) {}
};

class MST {
public:

    double totalDistance = 0;

    std::vector<primsTable> primsTables;

    void primsAlgorithm(const std::vector<Vertex> &vertices, int mapSize, bool MST_MODE);

    double distanceSquared(const Vertex& v1, const Vertex& v2);

    bool isAdjacent(const Vertex& v1, const Vertex& v2);

    void printPrims();
};