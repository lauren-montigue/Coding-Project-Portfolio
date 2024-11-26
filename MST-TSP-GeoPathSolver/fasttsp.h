// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0

#include "drone.h"

#include <vector>
#include <string>
#include <limits>

class FASTTSP {
public:
    // continue pushing back new points (indices) into this vector
    // is a vector the best representation? will have to put points in between and shift over elements... maybe linked list
    std::vector <Vertex> pathTraveled;
    double totalDistance = 0;

    void arbitraryInsertion(const std::vector<Vertex> &vertices, int mapSize, bool FASTTSP_MODE);

    void printTSP();

    double distance(const Vertex &v1, const Vertex &v2);
};