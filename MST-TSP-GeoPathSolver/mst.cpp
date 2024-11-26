// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0

#include "mst.h"

#include <limits>
#include <vector>
#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

void MST::primsAlgorithm(const std::vector<Vertex> &vertices, int mapSize, bool MST_MODE)
{
    primsTables.reserve(mapSize);

    // Initialize primsTables with default-constructed elements
    primsTables = std::vector<primsTable>(mapSize);

    // set starting point d_v to zero
    primsTables[0].distance = 0;

    // loop v times (until every k_v is true) (outer loop)
    for (int i = 0; i < mapSize; ++i)
    {
        // Find vertex with the smallest tentative distance among vertices not yet visited
        int minIndex = -1;
        double minDistance = std::numeric_limits<double>::infinity();
        for (int v = 0; v < mapSize; ++v) {
            if (!primsTables[v].beenVisited && primsTables[v].distance < minDistance) {
                minIndex = v;
                minDistance = primsTables[v].distance;
            }
        }

        // Mark the selected vertex as visited
        primsTables[minIndex].beenVisited = true;
        totalDistance += std::sqrt(primsTables[minIndex].distance);

        // Update distances and parents for vertices adjacent to the selected vertex
        for (int w = 0; w < mapSize; ++w) {
            // Check if vertex w is adjacent to the selected vertex and not yet visited
            if (!primsTables[w].beenVisited && (!MST_MODE || isAdjacent(vertices[minIndex], vertices[w]))) {
                // Calculate distance between vertices 
                double distanceVW = distanceSquared(vertices[w], vertices[minIndex]); // TODO: this is where you dont need to be doing sqrt every time
                // Update distance and parent if the calculated distance is smaller
                if (distanceVW < primsTables[w].distance) {
                    primsTables[w].distance = distanceVW;
                    primsTables[w].parentIdx = minIndex;
                }
            }
        }
    }

    if (MST_MODE)
        printPrims();
    
}

void MST::printPrims()
{
    // Output the total weight of the MST
    std::cout << std::fixed << std::setprecision(2);
    std::cout << totalDistance << '\n';

    // Output the edges in the MST
    for (size_t i = 1; i < primsTables.size(); ++i) {
        if (primsTables[i].parentIdx != -1) { // Check if there's a valid parentIdx
            // Ensure that the smaller index is printed first
            int smallerIdx = std::min(primsTables[i].parentIdx, static_cast<int>(i));
            int largerIdx = std::max(primsTables[i].parentIdx, static_cast<int>(i));
            std::cout << smallerIdx << " " << largerIdx << '\n';
        }
    }
} 

double MST::distanceSquared(const Vertex& v1, const Vertex& v2)
{
    return std::pow(v1.x - v2.x, 2) + std::pow(v1.y - v2.y, 2);
}

bool MST::isAdjacent(const Vertex& v1, const Vertex& v2) {
        // Check if vertices are in the same area
        if (v1.campusType == v2.campusType) {
            return true; // Vertices in the same area are adjacent
        }

        // Check if one vertex is a border location
        if (v1.campusType == whichCampus::kBorder || v2.campusType == whichCampus::kBorder) {
            return true; // One vertex is a border location, they are adjacent
        }

        // Otherwise, vertices are not adjacent
        return false;
    }
