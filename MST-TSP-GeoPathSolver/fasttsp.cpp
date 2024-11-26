// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0

#include "fasttsp.h"
#include "opttsp.h"

#include <vector>
#include <string>
#include <limits>
#include <cmath>
#include <iostream>
#include <iomanip>

void FASTTSP::arbitraryInsertion(const std::vector<Vertex> &vertices, int mapSize, bool FASTTSP_MODE)
{
    pathTraveled.reserve(mapSize + 1);

    // Start with the first two vertices
    pathTraveled.push_back(vertices[0]);
    pathTraveled.push_back(vertices[1]);
    pathTraveled.push_back(vertices[0]); // Return to starting vertex
    totalDistance += 2 * distance(vertices[0], vertices[1]);

    for (int i = 2; i < mapSize; ++i)
    {
        double minIncrease = std::numeric_limits<double>::max();
        size_t minIndex = -1;

        for (size_t j = 1; j < pathTraveled.size(); ++j) //TODO: check bounds, should be size -1?
        {
            // Calculate increase in distance if vertex i is inserted between j and j+1
            double increase = distance(pathTraveled[j - 1], vertices[i]) + 
                              distance(vertices[i], pathTraveled[j]) - 
                              distance(pathTraveled[j - 1], pathTraveled[j]);

            if (increase < minIncrease)
            {
                minIncrease = increase;
                minIndex = j;
            }
        }
        
        // Insert vertex i at the optimal position
        pathTraveled.insert(pathTraveled.begin() + minIndex, vertices[i]);
        totalDistance += minIncrease;
    }

    if (FASTTSP_MODE)
        printTSP();
}

void FASTTSP::printTSP()
{
    // Output the total weight of the TSP
    std::cout << std::fixed << std::setprecision(2);
    std::cout << totalDistance << '\n';

    for (size_t i = 0; i < pathTraveled.size() - 1; ++i)
    {
        std::cout << pathTraveled[i].vertexIdx << " ";
    }
    
    std::cout << '\n';
}

double FASTTSP::distance(const Vertex &v1, const Vertex &v2)
{
    return std::sqrt(std::pow(v1.x - v2.x, 2) + std::pow(v1.y - v2.y, 2));
}