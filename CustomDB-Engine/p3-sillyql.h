// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#ifndef SILLYQL_H
#define SILLYQL_H

#include <getopt.h> // Include getopt_long() function
#include "TableEntry.h"
#include "Table.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

/*struct Options {
    // Add struct members here as needed
};*/


class SillyQL {
public:
    std::unordered_map<std::string, Table> tableHashMap;
    bool quiet = false; //bools default to false

    void printHelp(char *argv[]);
    void getOptions(int argc, char *argv[]/*, Options &options*/);
    void readInput();

    void create(std::string &tableName);
    void remove(std::string &tableName);
    void insert(std::string &tableName);
    void print(std::string &tableName);
    void deleteFrom(std::string &tableName);
    void generate(std::string &tableName);
    void join(std::string &tableName1, std::string &tableName2);
};

#endif // SILLYQL_H