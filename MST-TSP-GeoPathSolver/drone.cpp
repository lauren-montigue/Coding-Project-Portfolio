// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0

// notes from video
// part B - do research online

// PART A
// for autograder, look for word hint. will be super useful
// output can be different and still correct
// output for part A will happen in the order that edges were marked as true
// distance from normal to medical or vice versa should be infinity
// that way it will be ignored by MST. it doesnt need to know about these locations
// determining if point is in normal, border, or medical
// when read x and y, store location along with it. x and y locations and enum type
// when reading input, dont have one struct that includes x y category and everything in print
// just have one for x y category so just have one set of code to read in the input
// then separate structure for prims table with d and k and p
// vector of inputs, xy category
// vector of prims table stuff if you need
// can convert to double as doing calculations
// make sure not to overflow int... cast to double before finish with math

// find first vertex, set to zero then find smallest unvisited vertex distance?
// then update that vertex to true
// then calcualte the distance between newly added vertex and unvisited vertices?

// PART B

// do google search for heuristics (tsp)
// heuristic is way of solving problem thats not necessarily best solution but is fast (do the nearest neighbor)
// will lose points if path length is too much longer than solution
// must be O(n^2)
// all heuristics are greedy in some way - but how is each greedy? simplest greedy is greedy nearest neighbor
// nearest neighbor picks one point arbitrarily then of all points, pick closest. might produce crossings - not good enough
// can add to it, use two opt (but wont get you all the points)
// if path length is reasonable, can start on part C

// PART C

// can make a specific type and get rid of template once you decide what you're putting in path vector
// make part C a class
// make genperms a member function of that class
// genperms is recursive but not tail recursive, make size_t permlength the only parameter
// everything else will be member variables
// add another double for running total
// because every time you run genperms, you want to get estimate of remaining portion
// in for loop in genperm put length += and length -=
// make sure to get double indexing right
// when you first call genperms, want to start a path at 0
// never make a call where its in any other position. 34 factorial versus 35 factorial. huge difference
// running total - make sure you add the closing edge


// OFFICE HOURS NOTES

// two / three for loops

// four vectors - k v d p

// have an enum for each coord - central, medical, border (return inf for medical to central)

// only sqrt at end when adding final distance totals

// can keep running total for all weights instead of another for loop 

// for part C, starting benchmark will be fasttsp from part B, for branch and bound

// part B look at nearest neighbor and arbitrary insert (nearest neighbor will use same data structures)

// part B: nearest neighbor with 2 opt

// write promising function for C, heuristic, how to estimate

// genperms locks in a point then locks in another, uses mst to find optimal next point
// generates arms - which way to best connect fixed and mst of permutation
// backtrack if neither arm matches up with lowerbound

// bugs basically coming from part A

// all medical and central, no border, no MST (edge case for test), one boundary, make sure that the border is only around cartesian coordinate 3

// dont store MST, just do another loop to print out edge

// for fasttsp, cycle, youre going to have vector of which nodes you jump from, add next best node

#include <getopt.h>
#include <iostream>
#include <string>
#include <cmath>

#include "drone.h"
#include "mst.h"
#include "fasttsp.h"
#include "opttsp.h"

using namespace std;

// TODO print actually helpful message here
void Drone::printHelp(char *argv[]) {
      cout << "Usage: " << argv[0] << " [-m resize|reserve|nosize] | -h\n";
      cout << "This program is to help you learn command-line processing,\n";
      cout << "reading data into a vector, the difference between resize and\n";
      cout << "reserve and how to properly read until end-of-file." << endl;
    }  // printHelp()

    void Drone::getOptions(int argc, char * argv[], Options &options) {
    // These are used with getopt_long()
    opterr = false; // Let us handle all error output for command line options
    int choice;
    int index = 0;
    option long_options[] = {
      { "help", no_argument, nullptr, 'h'},
      { "mode", required_argument, nullptr, 'm'},
      { nullptr, 0, nullptr, '\0' },
    };  // long_options[]

    //parse through getopt_long, make changes based on switch blocks

    while ((choice = getopt_long(argc, argv, "hm:", long_options, &index)) != -1) { //index needed, counts arguments

      string arg = ""; // not setting arg.. fix
      switch (choice) { //choice represents user input to command line
        case 'h':
        {
          printHelp(argv);
          exit(0);
        }

        case 'm': 
        {
          arg = optarg;

          if (options.whichModeIsIt != whichMode::kNone)
          {
            cerr << "Error: Must choose only one out of queue and stack modes" << endl;
            exit(1);
          }
          if (arg == "MST")
          {
            options.whichModeIsIt = whichMode::kMST;
            break;
          }
          else if (arg == "FASTTSP")
          {
            options.whichModeIsIt = whichMode::kFASTTSP;
            break;
          }
          else if (arg == "OPTTSP")
          {
            options.whichModeIsIt = whichMode::kOPTTSP;
            break;
          }
          else
          {
            // throw error
            // specified in spec
            break;
          }
        }

        default:
        {
          cerr << "Error: invalid option" << endl;
          exit(1); //covers the case of no argument
        }
      }  // switch ..choice

      

    }  // while

  }  // if ..mode

  void Drone::readInput(Options &options)
  {
    cin >> mapSize;
    vertices.reserve(mapSize);

    int x;
    int y;

    for (int i = 0; i < mapSize; ++i)
    {
      cin >> x;
      cin >> y;

      vertices.emplace_back(x, y, i);

      if (options.whichModeIsIt == whichMode::kMST)
      {
        if (y < 0 && x < 0)
        {
          vertices[i].campusType = whichCampus::kMedical;
          med_exists = true;
        }
        else if ((y == 0 && x <= 0) || (x == 0 && y <= 0))
        {
          vertices[i].campusType = whichCampus::kBorder;
          border_exists = true;
        }
        else
        {
          vertices[i].campusType = whichCampus::kMain;
          main_exists = true;
        }
      }

    }

    if (med_exists && main_exists && !border_exists)
    {
      cerr << "Cannot construct MST" << std::endl;
      exit(1);
    }

    if (options.whichModeIsIt == whichMode::kMST)
    {
      MST mst;
      mst.primsAlgorithm(vertices, mapSize, true);
    }
    else if (options.whichModeIsIt == whichMode::kFASTTSP)
    {
      FASTTSP fast;
      fast.arbitraryInsertion(vertices, mapSize, true);
    }
    else if (options.whichModeIsIt == whichMode::kOPTTSP) // or just else?
    {
      OPTTSP opt;
      opt.opt(vertices, mapSize);
    }
    
  }

 