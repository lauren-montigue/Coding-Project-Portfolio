// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

//meant to be built in pieces
//dont have to understand indices, join to start working on project
//can add code later to say if theres an index do whatever, otherwise do old thing
//starting point: command line - quiet and help
//then start reading input:
//start with a do while loop
// do { cout << "% ";
//std::cin >> cmd;
//PROCESS command
//while (cmd != QUIT)}
//dont getline a whole command, just read from std::cin
//then next write count (if you see # sign, you can do getline and throw rest away)

//then create a command
//remove is easy after you write create
//then insert and printall
//now you can see the project work!
//then deleterows and printwhere
//generate index and join

//high-level data structures - someone could create multiple tables at a time and work with them
//so need quick way to go from name of table to everything associated (hash tables)
//unordered map is associative container - key-value pairs
//key type is string (table name), value is table object
//whats in a table object (class)? table name, everything that came from... column names, column types, a way to remember the order (like model is index 1, etc)
//also need everything that comes from insert command in table object
//blue is meta data - data about data
//data in table is the black and white (in overview pic)
//high level: hash table with table objects
//hash tables are memory hogs... biggest memory comes from black and white (insert command), bigger than meta data
//col names will be small for example
//worry about space in insert command! dont let it grow by doubling. insert 1025 rows, 17 columns. if you
//let it grow naturally, itll double to 2048 and 32, 4x as much memory!
//use resize and reserve bby girl
//make size of other things reusable
//can also save some memory with index
//black and white should be a 2D std::vector, rows being outer dimension (first square brackets)
//table entry isnt template, its variant type - can take care of other types but will take care of things for you
//if you do things right, you'll never compare different types
//table entry class is provided in starter code
//when someone inserts, check if the table they want to insert into exists
//if table name is valid, then rest of line will be valid
//want to resize to n + k and want to put in rows from n to n + k - 1
//learn the difference between push back and emplace back
//push back adds x to the end of the std::vector. if you give emplace back a y, y is given to constructor for type x
//after insert, work on printall
//printall is big loop through all rows, all columns, but might not want to print all. maybe skipping over things
//then time for delete and printwhere
//for these, dont try and make them one function! break things down
//if you have table class, table class can have member functions
//print function, printall function, printwhere function that is also broken up
//dont want giant if else - break down into four way split of the type
//read the type then call helper with a table entry of the type
//printwhere figures out column name, type of col, does col exist, read a variable of that type 
//call helper with table entry containing that string, has to be right type
//then that function can do three-way split on comparison split. dont write this code three times. another helper!
//will eventually want to use functor for comparison

//DELETE
//dont have n^2 complexity, use STL - look at the algorithms (Canvas > files > resources > algo) get rid of things in a container based on some condition
//this is O(n)
//put things it likes at the beginning, leaves things it doesnt like untouched, then now you have to make it have less rows - use STL to do deleting
//STL will need functor to say do we like this? do we want to get rid of it? functor has to be able to accept a row
//functor has to know when i receive this row, i have to look at this value
//functor has to have member variables - which col to look in, value to compare to
//value is a table entry that contains the right type
//breakdwown: four-way split on type to read, call to helper with table entry containing what we read, then three-way split on what's the comparison
//then could make a functor, call another helper function with functor, then that function do the work... functor has to be templated bc could be one of three types
//take a look at set union for templated syntax
//functor itself has to initialize its member variables - look at index sort (similar approach)
//table entry objects cannot be default constructed, must all be created with particular type
//functor has to have constructor initializing member variable
//make three functors - less than, greater, equal.
//not a functor that does three jobs!

//PRINTWHERE
//draw out printwhere!
//this is hardest part, need to understand breakdown of four-way split, three-way split, then index gen
//how does generate index affect all these other commands? how to do efficiently?
//maybe gen along the way to make future commands faster
//BST index on price can help with less, equal, greater
//model - equality comparisons only, could do BST or hash. only BST does less than, greater

//GENERATE INDEX
//just have to go back after everything and check how it affects each function
//mayyybe will have to rewrite join, but that's it

//JOIN
//join two tables together
//example in ... where... spec I guess... of how this happens
//each is row combination... when name in one matches person in other, row gets produced
//if no matches, nothing is printed. output could be anything from 0 rows to m*m
//straightforward approach: loop through every row in first, every row in second
//what's better? if no matches, do it faster than n^2. this is where indices can help!

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

      //string arg = "";
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

      //office hours: why couldnt i declare into and tablename just in this if statement? so weird
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

  //FROM DARDEN LECTURE
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
  //std::cout << "made it to sillyQL print!" << std::endl;

  auto it = tableHashMap.find(tableName);
  if (it != tableHashMap.end()){

    int numColsToPrint;
    std::cin >> numColsToPrint;

    //std::cout << "num cols to print: " << numColsToPrint << std::endl;

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
      //std::cout << colName << std::endl;
      
      for (size_t j = 0; j < it->second.metaData.size(); ++j)
      {
        if (colName == it->second.metaData[j].colName)
        {
          colNames.push_back(colName);
          colIndices.push_back(j);
          //std::cout << j << std::endl;
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
      //std::cout << "calling print where!" << std::endl;
      it->second.printWhere(colIndices, colNames, quiet);
    }

    else
    {
      //std::cout << "calling print all!" << std::endl;
      //std::cout << "colNames.size() = " << colNames.size() << std::endl;
      //std::cout << "colIndices.size() = " << colIndices.size() << std::endl;
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