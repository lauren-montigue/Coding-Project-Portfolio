// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0

#include <vector>
#include <string>
#include <limits>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "opttsp.h" 
#include "mst.h"
#include "fasttsp.h"

// visualize tool on autograder 2
// can plot correct and your output

// FROM ALISON:

// generate fast tsp going into part C
// pop back 
// the way gen perms works is having full sequence, swapping last two, then second and third to last... basically going through all possible combos
// give it path from part B (that's the path to pass in, maybe a vertex version)
// code from ECAS session... if permutation length is equal to size, add to current path the distance from last one to 
// have to add on length of returning back to start point to compare to best
// bounding function is biggest part
// actual branch stuff is genperms

// integer N is length you're considering right now... can pass in N to end, can also just remake vector with those points
// genperms is doing swaps within growing vector
// start by passing 1??
// in that for loop, its self recursive and adds on 1 every time
// calculate sqrt as few times as possible, dont use for comparing distances, only use when adding to path length

void OPTTSP::opt(const std::vector<Vertex> &vertices, int mapSize)
{
  FASTTSP optfast;
  optfast.arbitraryInsertion(vertices, mapSize, false);

  opttsp_best_path_len = optfast.totalDistance;
  //std::cout << "total distance: " << optfast.totalDistance << std::endl;

  best_opttsp_path.reserve(mapSize + 1);
  opttsp_path.reserve(mapSize + 1);

  // Copy the pathTraveled vector to opttsp_path and best_opttsp_path
  opttsp_path = optfast.pathTraveled;
  best_opttsp_path = optfast.pathTraveled;

  // Remove the last point from both vectors
  opttsp_path.pop_back();
  best_opttsp_path.pop_back();

  //std::cout << "calling genPerms" << std::endl;
  genPerms(1); // 0 stays fixed

  printTSP();
}

void OPTTSP::genPerms(size_t permLength) {
  if (permLength == opttsp_path.size()) {
    
    // Add closing edge temporarily
    // Calculate the length of the current path
    double currentPathLength = opttsp_curr_path_len + std::sqrt(distanceSquared(opttsp_path[permLength - 1], opttsp_path[0]));

    // Define a small epsilon value
    // TODO: figure this part out... because it shouldn't have been evaluating to true regardless
    const double epsilon = 1e-6; // Adjust as needed

    // Update best path if the current path is shorter
    if (currentPathLength + epsilon < opttsp_best_path_len) {
        //std::cout << "found a better path!" << std::endl;

        // Output the total weight of the TSP
        /*std::cout << "current path length: " << currentPathLength << std::endl;
        std::cout << "previous best path length: " << opttsp_best_path_len << std::endl;

        for (size_t i = 0; i < opttsp_path.size(); ++i)
        {
            std::cout << opttsp_path[i].vertexIdx << " ";
        }
        
        std::cout << std::endl;*/

        opttsp_best_path_len = currentPathLength;
        best_opttsp_path = opttsp_path; 
    }

    return;
  }  // if ..complete path

  if (!promising(permLength)) {
    return;
  }  // if ..not promising

  for (size_t i = permLength; i < opttsp_path.size(); ++i) {
    std::swap(opttsp_path[permLength], opttsp_path[i]);

    double new_edge_len = std::sqrt(distanceSquared(opttsp_path[permLength], opttsp_path[permLength - 1]));
    opttsp_curr_path_len += new_edge_len;

    genPerms(permLength + 1);

    opttsp_curr_path_len -= new_edge_len;

    std::swap(opttsp_path[permLength], opttsp_path[i]);
  }  // for ..unpermuted elements
}  // genPerms()

bool OPTTSP::promising(size_t permLength){

  // Generate MST using Part A code
  std::vector<Vertex> subPath(opttsp_path.begin() + permLength, opttsp_path.end()); // Remaining vertices

  // Calculate arm lengths first since this is O(n)
  double firstArmLength = std::numeric_limits<double>::max();
  double lastArmLength = std::numeric_limits<double>::max();

  for (size_t i = 0; i < subPath.size(); ++i)
  {
    double firstDistanceSquared = distanceSquared(opttsp_path[0], subPath[i]);
    double lastDistanceSquared = distanceSquared(opttsp_path[permLength - 1], subPath[i]);

    if (firstDistanceSquared < firstArmLength)
        firstArmLength = firstDistanceSquared;

    if (lastDistanceSquared < lastArmLength)
        lastArmLength = lastDistanceSquared;
  }

  firstArmLength = std::sqrt(firstArmLength);
  lastArmLength = std::sqrt(lastArmLength);

  // Calculate lower bound
  double lowerBound = firstArmLength + lastArmLength + opttsp_curr_path_len;

  // Compare lower bound with best path length to see if you should stop early
  if (lowerBound >= opttsp_best_path_len) {
      return false; // Not promising
  }

  MST mst;

  // Reset totalDistance and primsTables
  mst.totalDistance = 0;
  mst.primsTables.clear();

  mst.primsAlgorithm(subPath, static_cast<int>(subPath.size()), false);

  lowerBound += mst.totalDistance;

  // Compare lower bound with best path length
  if (lowerBound >= opttsp_best_path_len) {
      return false; // Not promising
  }

  return true; // Promising
}

double OPTTSP::distanceSquared(const Vertex &v1, const Vertex &v2)
{
    return std::pow(v1.x - v2.x, 2) + std::pow(v1.y - v2.y, 2);
}

void OPTTSP::printTSP()
{
    // Output the total weight of the TSP
    std::cout << std::fixed << std::setprecision(2);
    std::cout << opttsp_best_path_len << '\n';

    for (size_t i = 0; i < best_opttsp_path.size(); ++i)
    {
        std::cout << best_opttsp_path[i].vertexIdx << " ";
    }
    
    std::cout << '\n';
}

// debug for the rest of the day... use your test cases and the spec to debug part C
// good luck!!