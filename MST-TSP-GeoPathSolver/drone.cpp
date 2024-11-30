// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0

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

 
