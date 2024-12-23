// Project Identifier: 19034C8F3B1196BF8E0C6E1C0F973D2FD550B88F


#include "p2-mine-escape.h"
#include "P2random.h"
#include <deque>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <iostream>
#include <getopt.h>
#include <sstream>
#include <iomanip>

using namespace std;

//Implement private member functions

//TODO print helpful message
void mineEscape::printHelp(char *argv[]) {
  std::cout << "Usage: " << argv[0] << " [-m resize|reserve|nosize] | -h\n";
  std::cout << "This program is to help you learn command-line processing,\n";
  std::cout << "reading data into a vector, the difference between resize and\n";
  std::cout << "reserve and how to properly read until end-of-file." << endl;
}  // printHelp()

void mineEscape::getOptions(int argc, char * argv[], Options &options) {
    // These are used with getopt_long()
    opterr = false; // Let us handle all error output for command line options
    int choice;
    int index = 0;
    option long_options[] = {
      { "help", no_argument, nullptr, 'h'},
      { "stats", required_argument, nullptr, 's'},
      { "median", no_argument, nullptr, 'm'},
      { "verbose", no_argument, nullptr, 'v'},
      { nullptr, 0, nullptr, '\0' },
    };  // long_options[]

    //parse through getopt_long, make changes based on switch blocks
    while ((choice = getopt_long(argc, argv, "hmvs:", long_options, &index)) != -1) { //index needed, counts arguments

      //string arg = "";
      switch (choice) { //choice represents user input to command line
        case 'h':
        {
          printHelp(argv);
          exit(0);
        }

        case 's': 
        {
          //cout << "stats mode" << endl;
          options.anythingExtraWhat.push_back(anythingExtra::kStatsFlag);
          if (optarg != nullptr) {
          statsArgument = std::stoi(optarg);
          } else {
              // Handle the case where optarg is null
              std::cerr << "Error: optarg is null." << std::endl;
              // Optionally, exit the program or set a default value for statsArgument
          }
          break;
        }

        case 'm':
        {
          //cout << "median mode" << endl;
          options.anythingExtraWhat.push_back(anythingExtra::kMedianFlag);
          break;
        }

        case 'v':
        {
          //cout << "verbose mode" << endl;
          options.anythingExtraWhat.push_back(anythingExtra::kVerboseFlag);
          break;
        }

      }  // if ..mode
    }
}

void mineEscape::readInput() {
    string mode;
    string junk;
    std::stringstream ss;

    // Read the mode indicator ("M" or "R")
    cin >> mode;

    if (mode == "M") {
        // Process Map mode

        // Read side length and starting position
        cin >> junk >> sideLength >> junk >> rowStart >> colStart;
        
        // Ensure the starting position is valid
        if (!(rowStart >= 0 && rowStart < static_cast<int>(sideLength)) ||
            !(colStart >= 0 && colStart < static_cast<int>(sideLength))) {
            cerr << "Error: Row or column number is out of bounds." << endl;
            exit(1);
        }

    } else if (mode == "R") {
        // Process Pseudorandom mode

        // Read parameters for pseudorandom generation
        uint32_t seed;
        uint32_t max_rubble;
        uint32_t likelihood_tnt_denominator;

        cin >> junk >> sideLength >> junk >> rowStart >> colStart >> junk >> seed >> junk >> max_rubble >> junk >> likelihood_tnt_denominator;

        // Ensure the starting position is valid
        if (!(rowStart >= 0 && rowStart < static_cast<int>(sideLength)) ||
            !(colStart >= 0 && colStart < static_cast<int>(sideLength))) {
            cerr << "Error: Row or column number is out of bounds." << endl;
            exit(1);
        }

        // Generate map based on pseudorandom parameters
        P2random::PR_init(ss, sideLength, seed, max_rubble, likelihood_tnt_denominator);
    } else {
        // Unknown mode
        cerr << "Error: Input file must be in either Map or Pseudorandom mode" << endl;
        exit(1);
    }

    istream &inputStream = (mode == "M") ? cin : ss;

    // Resize the vector of vectors to match the side length
    map2D.resize(sideLength, vector<mapEntry>(sideLength));

    // Read the grid entries
    for (size_t row = 0; row < sideLength; ++row) {
        for (size_t col = 0; col < sideLength; ++col) {
            int rubble;
            inputStream >> rubble;

            // Create and populate the mapEntry struct
            mapEntry entry;
            entry.num = rubble;

            // Push the entry into the vector of vectors
            map2D[row][col] = entry;
            //cout << "pushed back: " << entry.num << " at [" << row << "," << col << "]" << endl;
        }
    }
}

void mineEscape::balanceHeaps(priority_queue<int>& maxHeap, priority_queue<int, vector<int>, greater<int>>& minHeap) {
    if (!maxHeap.empty() && !minHeap.empty()) //neither are empty
    {
        while (maxHeap.size() > minHeap.size() + 1) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        }
        while (minHeap.size() > maxHeap.size() + 1) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }
    
    else if (maxHeap.empty() && minHeap.size() > 1) { //maxheap is empty, minheap is not
        while (minHeap.size() > maxHeap.size() + 1) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    } 
    
    else { //minheap is empty, maxheap is not
        while (maxHeap.size() > minHeap.size() + 1) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        }
    }
}

void mineEscape::getOut(const Options &options){

  //data structures for median
  std::priority_queue<int> medianMaxHeap; // Max heap for the lower half
  std::priority_queue<int, std::vector<int>, std::greater<int>> medianMinHeap;

  //temp cleared tile to push back onto deque
  std::deque<PQEntry> clearedTiles;

  //push starting position onto primary PQ and mark discovered
  primaryPQ.push(PQEntry(map2D[rowStart][colStart].num, rowStart, colStart));
  map2D[rowStart][colStart].beenDiscovered = true;

  uint32_t rowToInvestigate = rowStart;
  uint32_t colToInvestigate = colStart;
  int numInInvestigated = map2D[rowStart][colStart].num;

  //counts for verbose
  int numTilesCleared = 0;
  int amountRubbleCleared = 0;

  //reached edge condition
  bool reachedEdge = false;

  auto itStats = std::find(options.anythingExtraWhat.begin(), options.anythingExtraWhat.end(), anythingExtra::kStatsFlag);
  auto itMedian = std::find(options.anythingExtraWhat.begin(), options.anythingExtraWhat.end(), anythingExtra::kMedianFlag);

  //while priority queue is empty or reached an edge
  //inclusive ors ensure that corners are included
  while (!primaryPQ.empty() && !reachedEdge)
  {

    //if thing at top was pushed before it was later detonated by TNT and pushed again and discovered, ignore it
    if (map2D[primaryPQ.top().row][primaryPQ.top().col].beenInvestigated == true)
    {
      primaryPQ.pop();
    }

    //START OF INVESTIGATION OF TOP OF PRIMARYPQ

    rowToInvestigate = primaryPQ.top().row;
    colToInvestigate = primaryPQ.top().col;
    numInInvestigated = primaryPQ.top().rubble;

    //take out of priority queue
    primaryPQ.pop();

    //verbose mode

    auto itVerbose = std::find(options.anythingExtraWhat.begin(), options.anythingExtraWhat.end(), anythingExtra::kVerboseFlag);

    if (itVerbose != options.anythingExtraWhat.end() && map2D[rowToInvestigate][colToInvestigate].num > 0 && !reachedEdge)
    {
      std::cout << "Cleared: " << map2D[rowToInvestigate][colToInvestigate].num << " at [" << rowToInvestigate << "," << colToInvestigate << "]" << '\n';
    }    

    if (map2D[rowToInvestigate][colToInvestigate].num > 0 && !reachedEdge)
    {
      ++numTilesCleared;
      amountRubbleCleared += map2D[rowToInvestigate][colToInvestigate].num;
    
      if (itStats != options.anythingExtraWhat.end())
      {
        clearedTiles.push_back(PQEntry(map2D[rowToInvestigate][colToInvestigate].num, rowToInvestigate, colToInvestigate));
      }

      //MEDIAN
      if (itMedian != options.anythingExtraWhat.end())
      {
        if (medianMinHeap.empty() && medianMaxHeap.empty())
        {
          medianMinHeap.push(map2D[rowToInvestigate][colToInvestigate].num);
        }
        else if (medianMaxHeap.empty()) //only median max heap empty
        {
          if (map2D[rowToInvestigate][colToInvestigate].num < medianMinHeap.top())
          {
            medianMaxHeap.push(map2D[rowToInvestigate][colToInvestigate].num);
          }
          else
          {
            medianMinHeap.push(map2D[rowToInvestigate][colToInvestigate].num);
          }
        }
        else  //neither are empty
        {
          if (map2D[rowToInvestigate][colToInvestigate].num < medianMinHeap.top())
          {
            medianMaxHeap.push(map2D[rowToInvestigate][colToInvestigate].num);
          }
          else
          {
            medianMinHeap.push(map2D[rowToInvestigate][colToInvestigate].num);
          }
        }

        balanceHeaps(medianMaxHeap, medianMinHeap);

        std::cout << std::fixed << std::setprecision(2);

        if (medianMaxHeap.size() > medianMinHeap.size())
        {
          std::cout << "Median difficulty of clearing rubble is: " << medianMaxHeap.top() * 1.0 << '\n';
        }
        else if (medianMinHeap.size() > medianMaxHeap.size())
        {
          std::cout << "Median difficulty of clearing rubble is: " << medianMinHeap.top() * 1.0 << '\n';
        }
        else
        {
          std::cout << "Median difficulty of clearing rubble is: " << (medianMinHeap.top() + medianMaxHeap.top())/2.0 << '\n';
        }
      }

    }

    

    //cout << "checked verbose mode" << endl;

    //now actually clear rubble and mark investigated
    map2D[rowToInvestigate][colToInvestigate].num = 0;
    map2D[rowToInvestigate][colToInvestigate].beenInvestigated = true;


    if (rowToInvestigate == 0 || rowToInvestigate == sideLength - 1 || colToInvestigate == 0 || colToInvestigate == sideLength - 1)
    {
      reachedEdge = true;
    }
      
    //CASE FOR TNT
    if (numInInvestigated == -1)
    {

      //verbose mode
      if (itVerbose != options.anythingExtraWhat.end() && !map2D[rowToInvestigate][colToInvestigate].beenPushedBackToTNT)
      {
        std::cout << "TNT explosion at [" << rowToInvestigate << "," << colToInvestigate << "]!" << '\n';
      }

      if (itStats != options.anythingExtraWhat.end() && !map2D[rowToInvestigate][colToInvestigate].beenPushedBackToTNT)
      {
        clearedTiles.push_back(PQEntry(-1, rowToInvestigate, colToInvestigate));
      }

      //make sure that current tnt tile cannot be pushed back again into tnt
      map2D[rowToInvestigate][colToInvestigate].beenPushedBackToTNT = true;

      

      //if current row is not zero, add above to TNT pq
      //AND if it hasn't been investigated
      if (rowToInvestigate != 0 && map2D[rowToInvestigate - 1][colToInvestigate].beenInvestigated == false
          && map2D[rowToInvestigate - 1][colToInvestigate].beenPushedBackToTNT == false)
      {
        TNTpq.push(PQEntry(map2D[rowToInvestigate - 1][colToInvestigate].num, rowToInvestigate - 1, colToInvestigate));
        map2D[rowToInvestigate - 1][colToInvestigate].beenPushedBackToTNT = true;
      }

      //if current row is not last row, add below to TNT pq
      if (rowToInvestigate != sideLength - 1 && map2D[rowToInvestigate + 1][colToInvestigate].beenInvestigated == false
          && map2D[rowToInvestigate + 1][colToInvestigate].beenPushedBackToTNT == false)
      {
        TNTpq.push(PQEntry(map2D[rowToInvestigate + 1][colToInvestigate].num, rowToInvestigate + 1, colToInvestigate));
        map2D[rowToInvestigate + 1][colToInvestigate].beenPushedBackToTNT = true;
      }

      //if current col is not first, add left to TNT pq
      if (colToInvestigate != 0 && map2D[rowToInvestigate][colToInvestigate - 1].beenInvestigated == false
        && map2D[rowToInvestigate][colToInvestigate - 1].beenPushedBackToTNT == false)
      {
        TNTpq.push(PQEntry(map2D[rowToInvestigate][colToInvestigate - 1].num, rowToInvestigate, colToInvestigate - 1));
        map2D[rowToInvestigate][colToInvestigate - 1].beenPushedBackToTNT = true;
      }

      //if current col is not last, add right to TNT pq
      if (colToInvestigate != sideLength - 1 && map2D[rowToInvestigate][colToInvestigate + 1].beenInvestigated == false
        && map2D[rowToInvestigate][colToInvestigate + 1].beenPushedBackToTNT == false)
      {
        TNTpq.push(PQEntry(map2D[rowToInvestigate][colToInvestigate + 1].num, rowToInvestigate, colToInvestigate + 1));
        map2D[rowToInvestigate][colToInvestigate + 1].beenPushedBackToTNT = true;

      }

      //okay, now added all neighbors to TNT pq to be detonated
      while (!TNTpq.empty())
      {
        //cout << "now looping through tnt pq" << endl;

        uint32_t tntRowToInvestigate = TNTpq.top().row;
        uint32_t tntColToInvestigate = TNTpq.top().col;
        int32_t tntNumInInvestigated = TNTpq.top().rubble;

        //now pop from TNTpq
        //cout << "popping from tnt pq" << endl;
        TNTpq.pop();

        //push top of TNT priority queue onto normal priority queue because its now discovered
        primaryPQ.push(PQEntry(0, tntRowToInvestigate, tntColToInvestigate));


        //verbose mode
        if (itVerbose != options.anythingExtraWhat.end() && map2D[tntRowToInvestigate][tntColToInvestigate].num > 0)
        {
          std::cout << "Cleared by TNT: " << map2D[tntRowToInvestigate][tntColToInvestigate].num << " at [" << tntRowToInvestigate << "," << tntColToInvestigate << "]" << '\n';
        } 

        if (map2D[tntRowToInvestigate][tntColToInvestigate].num > 0)
        {
          ++numTilesCleared;
          amountRubbleCleared += map2D[tntRowToInvestigate][tntColToInvestigate].num;

          //STATS
          if (itStats != options.anythingExtraWhat.end())
          {
            clearedTiles.push_back(PQEntry(map2D[tntRowToInvestigate][tntColToInvestigate].num, tntRowToInvestigate, tntColToInvestigate));
          }

          //MEDIAN
          if (itMedian != options.anythingExtraWhat.end())
          {
            if (medianMinHeap.empty() && medianMaxHeap.empty())
            {
              medianMinHeap.push(map2D[tntRowToInvestigate][tntColToInvestigate].num);
            }
            else if (medianMaxHeap.empty()) //only median max heap empty
            {
              if (map2D[tntRowToInvestigate][tntColToInvestigate].num < medianMinHeap.top())
              {
                medianMaxHeap.push(map2D[tntRowToInvestigate][tntColToInvestigate].num);
              }
              else
              {
                medianMinHeap.push(map2D[tntRowToInvestigate][tntColToInvestigate].num);
              }
            }
            else  //neither are empty
            {
              if (map2D[tntRowToInvestigate][tntColToInvestigate].num < medianMinHeap.top())
              {
                medianMaxHeap.push(map2D[tntRowToInvestigate][tntColToInvestigate].num);
              }
              else
              {
                medianMinHeap.push(map2D[tntRowToInvestigate][tntColToInvestigate].num);
              }
            }

            balanceHeaps(medianMaxHeap, medianMinHeap);

            std::cout << std::fixed << std::setprecision(2);

            if (medianMaxHeap.size() > medianMinHeap.size())
            {
              std::cout << "Median difficulty of clearing rubble is: " << medianMaxHeap.top() * 1.0 << '\n';
            }
            else if (medianMinHeap.size() > medianMaxHeap.size())
            {
              std::cout << "Median difficulty of clearing rubble is: " << medianMinHeap.top() * 1.0 << '\n';
            }
            else
            {
              std::cout << "Median difficulty of clearing rubble is: " << (medianMinHeap.top() + medianMaxHeap.top())/2.0 << '\n';
            }
          }
        }


        //make sure the change is reflected on the map
        map2D[tntRowToInvestigate][tntColToInvestigate].num = 0;
        map2D[tntRowToInvestigate][tntColToInvestigate].beenDiscovered = true;


        if (tntNumInInvestigated == -1)
        {

          //verbose mode
          if (itVerbose != options.anythingExtraWhat.end())
          {
            std::cout << "TNT explosion at [" << tntRowToInvestigate << "," << tntColToInvestigate << "]!" << '\n';
          }

          if (itStats != options.anythingExtraWhat.end())
          {
            clearedTiles.push_back(PQEntry(-1, tntRowToInvestigate, tntColToInvestigate));
          }

          if (tntRowToInvestigate != 0 && map2D[tntRowToInvestigate - 1][tntColToInvestigate].beenInvestigated == false
              && map2D[tntRowToInvestigate - 1][tntColToInvestigate].beenPushedBackToTNT == false)
          {

            TNTpq.push(PQEntry(map2D[tntRowToInvestigate - 1][tntColToInvestigate].num, tntRowToInvestigate - 1, tntColToInvestigate));
            map2D[tntRowToInvestigate - 1][tntColToInvestigate].beenPushedBackToTNT = true;

          }

          //if current row is not last row, add below to TNT pq
          //TODO: currently can push two of same block onto TNT pq twice, need guard against this
          if (tntRowToInvestigate != sideLength - 1 && map2D[tntRowToInvestigate + 1][tntColToInvestigate].beenInvestigated == false
              && map2D[tntRowToInvestigate + 1][tntColToInvestigate].beenPushedBackToTNT == false)
          {
            TNTpq.push(PQEntry(map2D[tntRowToInvestigate + 1][tntColToInvestigate].num, tntRowToInvestigate + 1, tntColToInvestigate));
            map2D[tntRowToInvestigate + 1][tntColToInvestigate].beenPushedBackToTNT = true;

          }

          //if current col is not first, add left to TNT pq
          if (tntColToInvestigate != 0 && map2D[tntRowToInvestigate][tntColToInvestigate - 1].beenInvestigated == false
              && map2D[tntRowToInvestigate][tntColToInvestigate - 1].beenPushedBackToTNT == false)
          {
            TNTpq.push(PQEntry(map2D[tntRowToInvestigate][tntColToInvestigate - 1].num, tntRowToInvestigate, tntColToInvestigate - 1));
            map2D[tntRowToInvestigate][tntColToInvestigate - 1].beenPushedBackToTNT = true;

          }

          //if current col is not last, add right to TNT pq
          if (tntColToInvestigate != sideLength - 1 && map2D[tntRowToInvestigate][tntColToInvestigate + 1].beenInvestigated == false
              && map2D[tntRowToInvestigate][tntColToInvestigate + 1].beenPushedBackToTNT == false)
          {
            TNTpq.push(PQEntry(map2D[tntRowToInvestigate][tntColToInvestigate + 1].num, tntRowToInvestigate, tntColToInvestigate + 1));
            map2D[tntRowToInvestigate][tntColToInvestigate + 1].beenPushedBackToTNT = true;

          }
        }

        
              
      }
      
    }

    //CASE THAT ITS NOT TNT
    else
    {

      if (rowToInvestigate != 0 && map2D[rowToInvestigate - 1][colToInvestigate].beenDiscovered == false)
      {
        primaryPQ.push(PQEntry(map2D[rowToInvestigate - 1][colToInvestigate].num,rowToInvestigate - 1,colToInvestigate));
        map2D[rowToInvestigate - 1][colToInvestigate].beenDiscovered = true;
      }

      if (rowToInvestigate != sideLength - 1 && map2D[rowToInvestigate + 1][colToInvestigate].beenDiscovered == false)
      {
        primaryPQ.push(PQEntry(map2D[rowToInvestigate + 1][colToInvestigate].num, rowToInvestigate + 1, colToInvestigate));
        map2D[rowToInvestigate + 1][colToInvestigate].beenDiscovered = true;
      }

      if (colToInvestigate != 0 && map2D[rowToInvestigate][colToInvestigate - 1].beenDiscovered == false)
      {
        primaryPQ.push(PQEntry(map2D[rowToInvestigate][colToInvestigate - 1].num, rowToInvestigate, colToInvestigate - 1));
        map2D[rowToInvestigate][colToInvestigate - 1].beenDiscovered = true;
      }

      if (colToInvestigate != sideLength - 1 && map2D[rowToInvestigate][colToInvestigate + 1].beenDiscovered == false)
      {
        primaryPQ.push(PQEntry(map2D[rowToInvestigate][colToInvestigate + 1].num, rowToInvestigate, colToInvestigate + 1));
        map2D[rowToInvestigate][colToInvestigate + 1].beenDiscovered = true;
      }
    }
    

  }

  std::cout << "Cleared " << numTilesCleared << " tiles containing " << amountRubbleCleared << " rubble and escaped." << endl;

  //STATS OUTPUT
  if (itStats != options.anythingExtraWhat.end())
  {
    //print first tiles cleared
    std::cout << "First tiles cleared:" << endl;
    for (int i = 0; i < statsArgument && i < static_cast<int>(clearedTiles.size()); ++ i)
    {
      if (clearedTiles[i].rubble == -1)
      {
        std::cout << "TNT at [" << clearedTiles[i].row << "," << clearedTiles[i].col << "]" << endl;
      }
      else
      {
        std::cout << clearedTiles[i].rubble << " at [" << clearedTiles[i].row << "," << clearedTiles[i].col << "]" << endl;
      }
    }

    std::cout << "Last tiles cleared:" << endl;
    for (int i = 0; i < statsArgument && i < static_cast<int>(clearedTiles.size()); ++ i)
    {

      if (clearedTiles[clearedTiles.size() - i - 1].rubble == -1)
      {
        std::cout << "TNT at [" << clearedTiles[clearedTiles.size() - i - 1].row << "," << clearedTiles[clearedTiles.size() - i - 1].col << "]" << endl;
      }
      else
      {
        std::cout << clearedTiles[clearedTiles.size() - i - 1].rubble << " at [" << clearedTiles[clearedTiles.size() - i - 1].row << "," << clearedTiles[clearedTiles.size() - i - 1].col << "]" << endl;
      }
    }

    std::sort(clearedTiles.begin(), clearedTiles.end(), PQCompare());

    //easiest tiles
    std::cout << "Easiest tiles cleared:" << endl;
    for (int i = 0; i < statsArgument && i < static_cast<int>(clearedTiles.size()); ++ i)
    {

      if (clearedTiles[clearedTiles.size() - i - 1].rubble == -1)
      {
        std::cout << "TNT at [" << clearedTiles[clearedTiles.size() - i - 1].row << "," << clearedTiles[clearedTiles.size() - i - 1].col << "]" << endl;
      }
      else
      {
        std::cout << clearedTiles[clearedTiles.size() - i - 1].rubble << " at [" << clearedTiles[clearedTiles.size() - i - 1].row << "," << clearedTiles[clearedTiles.size() - i - 1].col << "]" << endl;
      }
    }
    

    //hardest tiles
    std::cout << "Hardest tiles cleared:" << endl;
    for (int i = 0; i < statsArgument && i < static_cast<int>(clearedTiles.size()); ++ i)
    {

      if (clearedTiles[i].rubble == -1)
      {
        cout << "TNT at [" << clearedTiles[i].row << "," << clearedTiles[i].col << "]" << endl;
      }
      else
      {
        cout << clearedTiles[i].rubble << " at [" << clearedTiles[i].row << "," << clearedTiles[i].col << "]" << endl;
      }
    }
  }

}




