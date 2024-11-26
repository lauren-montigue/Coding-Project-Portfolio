// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0

#ifndef OPTTSP_H
#define OPTTSP_H

#include <vector>
#include <string>
#include <limits>

#include "drone.h"

class OPTTSP {
public:

    double opttsp_best_path_len = 0;
    double opttsp_curr_path_len = 0; // running total
    std::vector<Vertex> opttsp_path;
    std::vector<Vertex> best_opttsp_path;

    void opt(const std::vector<Vertex> &vertices, int mapSize);

    void genPerms(size_t permLength);

    bool promising(size_t permLength);

    double distanceSquared(const Vertex &v1, const Vertex &v2);

    void printTSP();
};

#endif // OPTTSP