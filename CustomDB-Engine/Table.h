// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#ifndef TABLE_H
#define TABLE_H

#include <getopt.h> // Include getopt_long() function
#include "TableEntry.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>

// Struct to represent a column in a table
struct Column {
    std::string colName;
    EntryType colType;

    Column(const std::string& name, EntryType type) : colName(name), colType(type) {}
};

enum class IndexType:uint8_t{
	HASH,
	BST,
	NONE
};

struct GreaterThan {
    // Member variables
    size_t colIdx;
    TableEntry valueToCompare;

    // Constructor
    GreaterThan(size_t colIdx, const TableEntry& valueToCompare) 
        : colIdx(colIdx), valueToCompare(valueToCompare) {}

    // Overloaded operator
    bool operator()(const std::vector<TableEntry>& row) const {
        // Compare the entry at colIdx with valueToCompare
        return row[colIdx] > valueToCompare;
    }
};

struct LessThan {
    // Member variables
    size_t colIdx;
    TableEntry valueToCompare;

    // Constructor
    LessThan(size_t colIdx, const TableEntry& valueToCompare) 
        : colIdx(colIdx), valueToCompare(valueToCompare) {}

    // Overloaded operator
    bool operator()(const std::vector<TableEntry>& row) const {
        // Compare the entry at colIdx with valueToCompare
        return row[colIdx] < valueToCompare;
    }
};

struct EqualTo {
    // Member variables
    size_t colIdx;
    TableEntry valueToCompare;

    // Constructor
    EqualTo(size_t colIdx, const TableEntry& valueToCompare) 
        : colIdx(colIdx), valueToCompare(valueToCompare) {}

    // Overloaded operator
    bool operator()(const std::vector<TableEntry>& row) const {
        // Compare the entry at colIdx with valueToCompare
        return row[colIdx] == valueToCompare;
    }
};

class Table {
public:
    std::string name;
    int numCols;
    int numRows; 
    std::vector<Column> metaData;
    std::vector<std::vector<TableEntry>> data;
    std::unordered_map<TableEntry, std::vector<size_t>> indexHash;
    std::map<TableEntry, std::vector<size_t>> indexBST;
    IndexType indextype = IndexType::NONE;
    size_t colAssociatedWithIndex;

    //variable indicating which case were in - no index, hash index, bst index
    //index only appears on certain columns
    //another variable indicating which column
    //another unordered map<tableEntry, X> hashidx
    // std;::map<tsbleEntry, X> bstIdx
    // think about how these cooperate with printwhere

    // Constructor to initialize the table with a given number of columns
    Table(const std::string& tableName, int numCols);

    void create();

    void insert(int numRowsInserting);

    void printRow(const std::vector<size_t> &indicesToPrint, size_t currentRowIdx, bool quiet);

    bool printWhereHelper(const std::string& op, TableEntry value, TableEntry valueToCompare);

    void printAll(const std::vector<std::string> &listOfColNames, const std::vector<size_t> &indicesToPrint, bool quiet);

    void printWhere(const std::vector<size_t> &indicesToPrint, const std::vector<std::string> &colNamesToPrint, bool quiet);

    int deleteRowIf(size_t colIdx, TableEntry &valueToCompare, const std::string &op);

    void deleteFrom();

    void generate();

    void join(const Table& table2, bool quiet);
};

#endif // TABLE_H