// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include "p3-sillyql.h"
#include <getopt.h>
#include "TableEntry.h"
#include "Table.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

//TODO print helpful message
void SillyQL::printHelp(char *argv[]) {
  std::cout << "Usage: " << argv[0] << " [-m resize|reserve|nosize] | -h\n";
  std::cout << "This program is to help you learn command-line processing,\n";
  std::cout << "reading data into a vector, the difference between resize and\n";
  std::cout << "reserve and how to properly read until end-of-file." << std::endl;
}  // printHelp()

void SillyQL::getOptions(int argc, char * argv[]/*, Options &options*/) {
    // These are used with getopt_long()
    opterr = false; // Let us handle all error output for command line options
    int choice;
    int index = 0;
    option long_options[] = {
      { "help", no_argument, nullptr, 'h'},
      { "quiet", no_argument, nullptr, 'q'},
      { nullptr, 0, nullptr, '\0' },
    };  // long_options[]

    //parse through getopt_long, make changes based on switch blocks
    while ((choice = getopt_long(argc, argv, "hq", long_options, &index)) != -1) { //index needed, counts arguments

      switch (choice) { //choice represents user input to command line
        case 'h':
        {
          printHelp(argv);
          exit(0);
        }

        case 'q': 
        {
          quiet = true;
          break;
        }

      }  // if ..mode
    }
}

void SillyQL::readInput(){

  std::string command; 
  std::string into;
  std::string tableName;
  std::string from;

  do{
    std::cout << "% ";
    //std::cout << "lets cin something new" << std::endl;
    std::cin >> command; 
    //std::cout << command << std::endl;

    if (command[0] == '#')
    {
      std::getline(std::cin, command);
      continue; //go back to the top of the loop
    }

    else if (command == "CREATE")
    {
      std::string tableName;
      std::cin >> tableName;

      create(tableName);
    }

    else if (command == "REMOVE")
    {
      std::cin >> tableName;

      remove(tableName);
    }

    else if (command == "INSERT")
    {
      std::cin >> into;
      std::cin >> tableName; 

      insert(tableName);
    }
    
    else if (command == "PRINT")
    {
      std::cin >> from;
      std::cin >> tableName;

      //std::cout << "table name: " << tableName << std::endl;

      print(tableName);
    }

    else if (command == "DELETE")
    {
      std::string from;
      std::cin >> from;

      std::string tableName;
      std::cin >> tableName;

      deleteFrom(tableName);
    }

    else if (command == "JOIN")
    {
      //std::cout << "calling join" << std::endl;

      std::string tableName1;
      std::string theWordAnd;
      std::string tableName2;

      std::cin >> tableName1;
      std::cin>> theWordAnd;
      std::cin >> tableName2;


      join(tableName1, tableName2);

    }

    else if (command == "GENERATE")
    {
      std::string theWordFor;
      std::cin >> theWordFor;

      std::string tableName;
      std::cin >> tableName;

      generate(tableName);
    }

    else if (command == "QUIT")
    {
      std::cout << "Thanks for being silly!" << std::endl;
    }

    else 
    {
      std::string junk;
      std::getline(std::cin, junk);

      std::cout << "Error: unrecognized command" << '\n';
    }
  }
  while(command != "QUIT");
}

void SillyQL::create(std::string &tableName){

  auto it = tableHashMap.find(tableName);
  if (it == tableHashMap.end())
  {
    int numCols;
    std::cin >> numCols;

    Table table(tableName, numCols);

    table.create(); 

    tableHashMap.insert({tableName, table});
  }
  else
  {
    std::string junk;
    std::getline(std::cin, junk);

    std::cout << "Error during CREATE: Cannot create already existing table " << tableName << std::endl;
    return;
  }
}

void SillyQL::remove(std::string &tableName){
  auto it = tableHashMap.find(tableName);
  if (it != tableHashMap.end())
  {
    tableHashMap.erase(it);
    std::cout << "Table " << tableName << " removed" << '\n';
  }
  else
  {
    std::cout << "Error during REMOVE: " << tableName << " does not name a table in the database" << '\n';
    return;
  }
}

void SillyQL::insert(std::string &tableName)
{
  //std::cout << "made it to sillyQL insert" << std::endl;

  //std::cout << "table name is:" << tableName << "got it?" << std::endl;

  int numRowsInserting;
  std::cin >> numRowsInserting;

  //std::cout << "num rows: " << numRowsInserting << " ";

  std::string rows;
  std::cin >> rows;

  //std::cout << "rows: " << rows << std::endl;

  auto it = tableHashMap.find(tableName);
  if (it != tableHashMap.end())
  {
    //std::cout << "okay table is in hash map" << std::endl;

    int originalLength = it->second.numRows;

    it->second.insert(numRowsInserting);

    std::cout << "Added " << numRowsInserting << " rows to " << tableName << " from position " << originalLength << " to " << originalLength + numRowsInserting - 1 << '\n';
  }
  else
  {
    std::string junk;
    std::getline(std::cin, junk);

    std::cout << "Error during INSERT: " << tableName << " does not name a table in the database" << '\n';
    return;
  }
}

void SillyQL::print(std::string &tableName)
{
  auto it = tableHashMap.find(tableName);
  if (it != tableHashMap.end()){

    int numColsToPrint;
    std::cin >> numColsToPrint;

    std::vector <std::string> colNames;
    colNames.reserve(numColsToPrint);

    std::vector <size_t> colIndices;
    colIndices.reserve(numColsToPrint);

    std::string colName;

    bool matchFound;
    bool printedError;

    for (int i = 0; i < numColsToPrint; ++i)
    {
      std::cin >> colName;
      
      for (size_t j = 0; j < it->second.metaData.size(); ++j)
      {
        if (colName == it->second.metaData[j].colName)
        {
          colNames.push_back(colName);
          colIndices.push_back(j);
          matchFound = true;
        }
      }

      if (!matchFound && !printedError)
      {
        std::cout << "Error during PRINT: " << colName << " does not name a column in " << tableName << '\n';
        printedError = true;
      }

      matchFound = false;
    }

    std::string allOrWhere;
    std::cin >> allOrWhere;

    if (allOrWhere == "WHERE")
    {
      it->second.printWhere(colIndices, colNames, quiet);
    }

    else
    {
      it->second.printAll(colNames, colIndices, quiet);
    }

  }

  else //No table with that name
  {
    std::string junk;
    std::getline(std::cin, junk);

    std::cout << "Error during PRINT: " << tableName << " does not name a table in the database" << '\n';
    return;
  }
  
}

void SillyQL::deleteFrom(std::string &tableName)
{
  auto it = tableHashMap.find(tableName);
  if (it != tableHashMap.end())
  {
    it->second.deleteFrom();
  }
  else
  {
    std::string junk;
    std::getline(std::cin, junk);

    std::cout << "Error during DELETE: " << tableName << " does not name a table in the database" << '\n';
    return;

  }
}

void SillyQL::generate(std::string &tableName)
{
  auto it = tableHashMap.find(tableName);
  if (it != tableHashMap.end())
  {
    it->second.generate();
  }
  else
  {
    std::string junk;
    std::getline(std::cin, junk);

    std::cout << "Error during GENERATE: " << tableName << " does not name a table in the database" << '\n';
    return;

  }

}

void SillyQL::join(std::string &tableName1, std::string &tableName2)
{
  //std::cout << "called sillyQL join" << std::endl;

  auto it1 = tableHashMap.find(tableName1);
  auto it2 = tableHashMap.find(tableName2);

  if (it1 != tableHashMap.end() && it2 != tableHashMap.end())
  {
    //std::cout << "calling table join" << std::endl;
    it1->second.join(it2->second, quiet);
  }
  else if (it2 != tableHashMap.end())
  {
    std::string junk;
    std::getline(std::cin, junk);

    std::cout << "Error during JOIN: " << tableName1 << " does not name a table in the database" << '\n';
    return;
  }
  else
  {
    std::string junk;
    std::getline(std::cin, junk);

    std::cout << "Error during JOIN: " << tableName2 << " does not name a table in the database" << '\n';
    return;
  }

}
