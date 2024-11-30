// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include "Table.h"
#include "TableEntry.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

// Constructor implementation
Table::Table(const std::string& tableName, int numCols) : name(tableName), numCols(numCols), numRows(0) {
    metaData.reserve(numCols);
}

void Table::create()
{
    std::string colName;
    std::string colType;

    //read in col type and push back onto metaData vector 
    for (int i = 0; i < numCols; ++i)
    {
        //can read in as a string and see what its equal to, diff logic for different cases
        std::cin >> colType;

        if (colType == "bool")
        {
            metaData.emplace_back("default_name", EntryType::Bool); //constructs and pushes back
        }
        if (colType == "string")
        {
            metaData.emplace_back("default_name", EntryType::String); //constructs and pushes back
        }
        else if (colType == "int")
        {
            metaData.emplace_back("default_name", EntryType::Int); //constructs and pushes back
        }
        else if (colType == "double")
        {
            metaData.emplace_back("default_name", EntryType::Double); //constructs and pushes back
        }
    }

    std::cout << "New table " << name << " with column(s) ";

    //read in column names
    for (int i = 0; i < numCols; ++i)
    {
        std::cin >> colName;
        metaData[i].colName = colName;

        std::cout << metaData[i].colName << " ";
    }

    std::cout << "created" << '\n';
}

void Table::insert(int numRowsInserting)
{
    data.reserve(numRows + numRowsInserting);

     bool b;
     std::string s;
     int integer;
     double d;

    for (int i = 0; i < numRowsInserting; ++i) //looping through rows being inserted
    {
        std::vector <TableEntry> tempRow;
        tempRow.reserve(numCols);

        for (int j = 0; j < numCols; ++j)
        {
            //already know the type because of guarantee that no type mismatches, check type saved in metadata
            if (metaData[j].colType == EntryType::Bool)
            {
               
                std::cin >> b;

                TableEntry temp(b);
                tempRow.emplace_back(b);
            }
            else if (metaData[j].colType == EntryType::String)
            {
                std::cin >> s;

                TableEntry temp(s);
                tempRow.emplace_back(s);
            }
            else if (metaData[j].colType == EntryType::Int)
            {
                
                std::cin >> integer;

                TableEntry temp(integer);
                tempRow.emplace_back(integer);
            }
            else 
            {
                
                std::cin >> d;

                TableEntry temp(d);
                tempRow.emplace_back(d);
            }
        }
        // Append full row
        data.push_back(tempRow);
    }
    
    // update bst or hash if there is one:

    // Create the index based on the determined index type
    if (indextype == IndexType::HASH)
    {
        // Create a new hash index
        // Iterate through each row in 'data'
        for (size_t i = static_cast<size_t>(numRows); i < data.size(); ++i)
        {
            // Retrieve the corresponding value in 'colIndex' column for the current row
            TableEntry entry = data[i][colAssociatedWithIndex];

            // Update the hash index accordingly
            indexHash[entry].push_back(i);
        }
    }

    // Create the index based on the determined index type
    if (indextype == IndexType::BST)
    {
        // Create a new BST index
        // Iterate through each row in 'data'
        for (size_t i = static_cast<size_t>(numRows); i < data.size(); ++i)
        {
            // Retrieve the corresponding value in 'colIndex' column for the current row
            TableEntry entry = data[i][colAssociatedWithIndex];

            // Update the BST index accordingly
            indexBST[entry].push_back(i);
        }
    }

    numRows+= numRowsInserting;
}

void Table::printRow(const std::vector<size_t> &indicesToPrint, size_t currentRowIdx, bool quiet)
{
    if (!quiet){
        for (size_t j = 0; j < indicesToPrint.size(); ++j)
        {
            std::cout << data[currentRowIdx][indicesToPrint[j]] << " "; 
        }
        std::cout << '\n';
    }
}

bool Table::printWhereHelper(const std::string& op, TableEntry value, TableEntry valueToCompare)
{
    if (op == ">")
    {
        return (value > valueToCompare);
    }
    else if (op == "<")
    {
        return (value < valueToCompare);
    }
    else
    {
        return (value == valueToCompare);
    }
}

void Table::printAll(const std::vector<std::string> &listOfColNames, const std::vector<size_t> &indicesToPrint, bool quiet)
{

    if (!quiet)
    {
        for (size_t i = 0; i < listOfColNames.size(); ++i)
        {
            std::cout << listOfColNames[i] << " ";
        }

        std::cout << '\n';
    }

    for (int row = 0; row < numRows; ++row)
    {
        printRow(indicesToPrint, row, quiet);        
    }

    std::cout << "Printed " << numRows << " matching rows from " << name << '\n';
}

void Table::printWhere(const std::vector<size_t> &indicesToPrint, const std::vector<std::string> &listOfColNames, bool quiet)
{
    int matchingRows = 0;

    //std::cin remaining info
    std::string colName;
    std::cin >> colName;

    //find colIndex with that name

    size_t colIndex = 0;
    bool found;

    for (size_t j = 0; j < metaData.size(); ++j)
    {
        if (metaData[j].colName == colName)
        {
            colIndex = j;
            found = true;
            break;
        }
    }

    //error if no such column name
    if (!found)
    {
        std::string junk;
        std::getline(std::cin, junk);
        //erroring here when it shouldn't
        std::cout << "Error during PRINT: " << colName << " does not name a column in " << name << '\n';
        return;
    }

    if (!quiet)
    {
        for (size_t i = 0; i < listOfColNames.size(); ++i)
        {
            std::cout << listOfColNames[i] << " ";
        }
        std::cout << '\n';
    }

    std::string op;
    std::cin >> op;
   
    //construct table entry using corresponding type constructor
    if (metaData[colIndex].colType == EntryType::Bool)
    {
        bool valueToCompare;
        std::cin >> valueToCompare;

        TableEntry tableValueToCompare(valueToCompare);

        // no associated index
        if (indextype == IndexType::NONE || colAssociatedWithIndex != colIndex || 
            (indextype == IndexType::HASH && op != "=")){
            for (size_t i = 0; i < static_cast<size_t>(numRows); ++i){

                if (printWhereHelper(op, data[i][colIndex], tableValueToCompare))
                {
                    printRow(indicesToPrint, i, quiet);
                    ++matchingRows;
                }
            }
        }
        else if (indextype == IndexType::HASH)
        {
            // Use hash index
            auto it = indexHash.find(tableValueToCompare);
            if (it != indexHash.end())
            {
                for (size_t i : it->second)
                {
                    printRow(indicesToPrint, i, quiet);
                        ++matchingRows;
                }
            }
        }
        else
        {
            // Use BST index
            if (op == ">")
            {
                auto lowBound = indexBST.lower_bound(tableValueToCompare);
                
                if (lowBound != indexBST.end() && !(tableValueToCompare < lowBound->first))
                {
                    ++lowBound;
                }

                if (lowBound != indexBST.end())
                {
                    auto it = lowBound;
                    while (it != indexBST.end()) {
                        for (size_t i : it->second) {
                            ++matchingRows;
                            printRow(indicesToPrint, i, quiet);
                        }
                        ++it;
                    }
                }
            }
            else if (op == "<") // less than
            {
                auto lowBound = indexBST.lower_bound(tableValueToCompare);
                
                auto it = indexBST.begin();
                while (it != lowBound) {
                    for (size_t i : it->second) {
                        ++matchingRows;
                        printRow(indicesToPrint, i, quiet);
                    }
                    ++it;
                }
                
            }
            else // equal to
            {
                auto it = indexBST.find(tableValueToCompare);

                if (it != indexBST.end())
                {
                    for (size_t i : it->second) {
                            ++matchingRows;
                            printRow(indicesToPrint, i, quiet);
                    }
                }
            }            
        }
    }
    else if (metaData[colIndex].colType == EntryType::String)
    {
        std::string valueToCompare;
        std::cin >> valueToCompare;

        TableEntry tableValueToCompare(valueToCompare);

        // no associated index
        if (indextype == IndexType::NONE || colAssociatedWithIndex != colIndex || 
            (indextype == IndexType::HASH && op != "=")){
            for (size_t i = 0; i < static_cast<size_t>(numRows); ++i){

                if (printWhereHelper(op, data[i][colIndex], tableValueToCompare))
                {
                    printRow(indicesToPrint, i, quiet);
                    ++matchingRows;
                }
            }
        }
        else if (indextype == IndexType::HASH)
        {
            // Use hash index
            auto it = indexHash.find(tableValueToCompare);
            if (it != indexHash.end())
            {
                for (size_t i : it->second)
                {
                    if (printWhereHelper(op, data[i][colIndex], tableValueToCompare))
                    {
                        printRow(indicesToPrint, i, quiet);
                        ++matchingRows;
                    }
                }
            }
        }
        else
        {
            // Use BST index
            if (op == ">")
            {
                auto lowBound = indexBST.lower_bound(tableValueToCompare);
                
                if (lowBound != indexBST.end() && !(tableValueToCompare < lowBound->first))
                {
                    ++lowBound;
                }

                if (lowBound != indexBST.end())
                {
                    auto it = lowBound;
                    while (it != indexBST.end()) {
                        for (size_t i : it->second) {
                            ++matchingRows;
                            printRow(indicesToPrint, i, quiet);
                        }
                        ++it;
                    }
                }
            }
            else if (op == "<") // less than
            {
                auto lowBound = indexBST.lower_bound(tableValueToCompare);
                auto it = indexBST.begin();
                while (it != lowBound) {
                    for (size_t i : it->second) {
                        ++matchingRows;
                        printRow(indicesToPrint, i, quiet);
                    }
                    ++it;
                }
            }
            else // equal to
            {
                auto it = indexBST.find(tableValueToCompare);

                if (it != indexBST.end())
                {
                    for (size_t i : it->second) {
                            ++matchingRows;
                            printRow(indicesToPrint, i, quiet);
                    }
                }
            } 
        }
    }
    else if (metaData[colIndex].colType == EntryType::Int)
    {
        int valueToCompare;
        std::cin >> valueToCompare;

        TableEntry tableValueToCompare(valueToCompare);

        // no associated index
        if (indextype == IndexType::NONE || colAssociatedWithIndex != colIndex || 
            (indextype == IndexType::HASH && op != "=")){
            for (size_t i = 0; i < static_cast<size_t>(numRows); ++i){

                if (printWhereHelper(op, data[i][colIndex], tableValueToCompare))
                {
                    printRow(indicesToPrint, i, quiet);
                    ++matchingRows;
                }
            }
        }
        else if (indextype == IndexType::HASH)
        {
            // Use hash index
            auto it = indexHash.find(tableValueToCompare);
            if (it != indexHash.end())
            {
                for (size_t i : it->second)
                {
                    if (printWhereHelper(op, data[i][colIndex], tableValueToCompare))
                    {
                        printRow(indicesToPrint, i, quiet);
                        ++matchingRows;
                    }
                }
            }
        }
        else
        {
            // Use BST index
            if (op == ">")
            {
                auto lowBound = indexBST.lower_bound(tableValueToCompare);
                
                if (lowBound != indexBST.end() && !(tableValueToCompare < lowBound->first))
                {
                    ++lowBound;
                }

                if (lowBound != indexBST.end())
                {
                    auto it = lowBound;
                    while (it != indexBST.end()) {
                        for (size_t i : it->second) {
                            ++matchingRows;
                            printRow(indicesToPrint, i, quiet);
                        }
                        ++it;
                    }
                }
            }
            else if (op == "<") // less than
            {
                auto lowBound = indexBST.lower_bound(tableValueToCompare);
                auto it = indexBST.begin();
                while (it != lowBound) {
                    for (size_t i : it->second) {
                        ++matchingRows;
                        printRow(indicesToPrint, i, quiet);
                    }
                    ++it;
                }
                
            }
            else // equal to
            {
                auto it = indexBST.find(tableValueToCompare);

                if (it != indexBST.end())
                {
                    for (size_t i : it->second) {
                            ++matchingRows;
                            printRow(indicesToPrint, i, quiet);
                    }
                }
            } 
        }
    }
    else 
    {
        double valueToCompare;
        std::cin >> valueToCompare;

        TableEntry tableValueToCompare(valueToCompare);

        // no associated index
        if (indextype == IndexType::NONE || colAssociatedWithIndex != colIndex || 
            (indextype == IndexType::HASH && op != "=")){
            for (size_t i = 0; i < static_cast<size_t>(numRows); ++i){

                if (printWhereHelper(op, data[i][colIndex], tableValueToCompare))
                {
                    printRow(indicesToPrint, i, quiet);
                    ++matchingRows;
                }
            }
        }
        else if (indextype == IndexType::HASH)
        {
            // Use hash index
            auto it = indexHash.find(tableValueToCompare);
            if (it != indexHash.end())
            {
                for (size_t i : it->second)
                {
                    if (printWhereHelper(op, data[i][colIndex], tableValueToCompare))
                    {
                        printRow(indicesToPrint, i, quiet);
                        ++matchingRows;
                    }
                }
            }
        }
        else
        {
            // Use BST index

            if (op == ">")
            {
                auto lowBound = indexBST.lower_bound(tableValueToCompare);

                if (lowBound != indexBST.end() &&!(tableValueToCompare < lowBound->first))
                {
                    ++lowBound;
                }

                if (lowBound != indexBST.end())
                {
                    auto it = lowBound;
                    while (it != indexBST.end()) {
                        for (size_t i : it->second) {
                            ++matchingRows;
                            printRow(indicesToPrint, i, quiet);
                        }
                        ++it;
                    }
                }
            }
            else if (op == "<") // less than
            {
                auto lowBound = indexBST.lower_bound(tableValueToCompare);
                auto it = indexBST.begin();
                while (it != lowBound) {
                    for (size_t i : it->second) {
                        ++matchingRows;
                        printRow(indicesToPrint, i, quiet);
                    }
                    ++it;
                }
                
            }
            else // equal to
            {
                auto it = indexBST.find(tableValueToCompare);

                if (it != indexBST.end())
                {
                    for (size_t i : it->second) {
                            ++matchingRows;
                            printRow(indicesToPrint, i, quiet);
                    }
                }
            } 
        }
    }
    
    //print summary
    std::cout << "Printed " << matchingRows << " matching rows from " << name << '\n';
}

int Table::deleteRowIf(size_t colIdx, TableEntry &valueToCompare, const std::string &op)
{
    int numRowsDeleted;

    if (op == ">")
    {
        GreaterThan g(colIdx, valueToCompare);
        auto newEnd = remove_if(data.begin(), data.end(), g); //remove_if returns iterator to new end
        numRowsDeleted = static_cast<int>(data.end() - newEnd);
        data.erase(newEnd, data.end());

        numRows = static_cast<int>(newEnd - data.begin());
    }
    else if (op == "<")
    {
        LessThan l(colIdx, valueToCompare);
        auto newEnd = remove_if(data.begin(), data.end(), l); //remove_if returns iterator to new end
        numRowsDeleted = static_cast<int>(data.end() - newEnd);
        data.erase(newEnd, data.end());

        numRows = static_cast<int>(newEnd - data.begin());
    }
    else
    {
        EqualTo e(colIdx, valueToCompare);
        auto newEnd = remove_if(data.begin(), data.end(), e); //remove_if returns iterator to new end
        numRowsDeleted = static_cast<int>(data.end() - newEnd);
        data.erase(newEnd, data.end());

        numRows = static_cast<int>(newEnd - data.begin());
    }

    return numRowsDeleted;
}

void Table::deleteFrom()
{
    //std::cin remaining info
    std::string where;
    std::cin >> where;

    std::string colName;
    std::cin >> colName;

    int numRowsDeleted;

    size_t colIndex = 0;
    bool found;

    for (size_t j = 0; j < metaData.size(); ++j)
    {
        if (metaData[j].colName == colName)
        {
            colIndex = j;
            found = true;
            break;
        }
    }

    //error if no such column name
    if (!found)
    {
        std::string junk;
        std::getline(std::cin, junk);
        std::cout << "Error during DELETE: " << colName << " does not name a column in " << name << '\n';
        return;
    }

    std::string op;
    std::cin >> op;

    if (metaData[colIndex].colType == EntryType::Bool)
    {
        bool valueToCompare;
        std::cin >> valueToCompare;

        TableEntry tableValueToCompare(valueToCompare);

        //TODO speed up by using hash or bst to make deletion faster in original data structure
        numRowsDeleted = deleteRowIf(colIndex, tableValueToCompare, op);

        // re-generate the hash or bst

        // Check if an index already exists for the specified column
        if (indextype == IndexType::HASH && !indexHash.empty())
        {
            // Discard the old hash index
            indexHash.clear();
        }
        else if (indextype == IndexType::BST && !indexBST.empty())
        {
            // Discard the old BST index
            indexBST.clear();
        }

        // Create the index based on the determined index type
        if (indextype == IndexType::HASH)
        {
            // Create a new hash index
            // Iterate through each row in 'data'
            for (size_t i = 0; i < data.size(); ++i)
            {
                // Retrieve the corresponding value in 'colIndex' column for the current row
                TableEntry entry = data[i][colAssociatedWithIndex];

                // Update the hash index accordingly
                indexHash[entry].push_back(i);
            }
        }

        // Create the index based on the determined index type
        if (indextype == IndexType::BST)
        {
            // Create a new BST index
            // Iterate through each row in 'data'
            for (size_t i = 0; i < data.size(); ++i)
            {
                // Retrieve the corresponding value in 'colIndex' column for the current row
                TableEntry entry = data[i][colAssociatedWithIndex];

                // Update the BST index accordingly
                indexBST[entry].push_back(i);
            }
        }

    }
    else if (metaData[colIndex].colType == EntryType::String)
    {
        std::string valueToCompare;
        std::cin >> valueToCompare;

        TableEntry tableValueToCompare(valueToCompare);

        //TODO speed up by using hash or bst to make deletion faster in original data structure
        numRowsDeleted = deleteRowIf(colIndex, tableValueToCompare, op);

        // Check if an index already exists for the specified column
        if (indextype == IndexType::HASH && !indexHash.empty())
        {
            // Discard the old hash index
            indexHash.clear();
        }
        else if (indextype == IndexType::BST && !indexBST.empty())
        {
            // Discard the old BST index
            indexBST.clear();
        }

        // Create the index based on the determined index type
        if (indextype == IndexType::HASH)
        {
            // Create a new hash index
            // Iterate through each row in 'data'
            for (size_t i = 0; i < data.size(); ++i)
            {
                // Retrieve the corresponding value in 'colIndex' column for the current row
                TableEntry entry = data[i][colAssociatedWithIndex];

                // Update the hash index accordingly
                indexHash[entry].push_back(i);
            }
        }

        // Create the index based on the determined index type
        if (indextype == IndexType::BST)
        {
            // Create a new BST index
            // Iterate through each row in 'data'
            for (size_t i = 0; i < data.size(); ++i)
            {
                // Retrieve the corresponding value in 'colIndex' column for the current row
                TableEntry entry = data[i][colAssociatedWithIndex];

                // Update the BST index accordingly
                indexBST[entry].push_back(i);
            }
        }
    }
    else if (metaData[colIndex].colType == EntryType::Int)
    {
        int valueToCompare;
        std::cin >> valueToCompare;

        TableEntry tableValueToCompare(valueToCompare);

        //TODO speed up by using hash or bst to make deletion faster in original data structure
        numRowsDeleted = deleteRowIf(colIndex, tableValueToCompare, op);

        // Check if an index already exists for the specified column
        if (indextype == IndexType::HASH && !indexHash.empty())
        {
            // Discard the old hash index
            indexHash.clear();
        }
        else if (indextype == IndexType::BST && !indexBST.empty())
        {
            // Discard the old BST index
            indexBST.clear();
        }

        // Create the index based on the determined index type
        if (indextype == IndexType::HASH)
        {
            // Create a new hash index
            // Iterate through each row in 'data'
            for (size_t i = 0; i < data.size(); ++i)
            {
                // Retrieve the corresponding value in 'colIndex' column for the current row
                TableEntry entry = data[i][colAssociatedWithIndex];

                // Update the hash index accordingly
                indexHash[entry].push_back(i);
            }
        }

        // Create the index based on the determined index type
        if (indextype == IndexType::BST)
        {
            // Create a new BST index
            // Iterate through each row in 'data'
            for (size_t i = 0; i < data.size(); ++i)
            {
                // Retrieve the corresponding value in 'colIndex' column for the current row
                TableEntry entry = data[i][colAssociatedWithIndex];

                // Update the BST index accordingly
                indexBST[entry].push_back(i);
            }
        }
    }
    else 
    {
        double valueToCompare;
        std::cin >> valueToCompare;

        TableEntry tableValueToCompare(valueToCompare);

        //TODO speed up by using hash or bst to make deletion faster in original data structure
        numRowsDeleted = deleteRowIf(colIndex, tableValueToCompare, op);

        // Check if an index already exists for the specified column
        if (indextype == IndexType::HASH && !indexHash.empty())
        {
            // Discard the old hash index
            indexHash.clear();
        }
        else if (indextype == IndexType::BST && !indexBST.empty())
        {
            // Discard the old BST index
            indexBST.clear();
        }

        // Create the index based on the determined index type
        if (indextype == IndexType::HASH)
        {
            // Create a new hash index
            // Iterate through each row in 'data'
            for (size_t i = 0; i < data.size(); ++i)
            {
                // Retrieve the corresponding value in 'colIndex' column for the current row
                TableEntry entry = data[i][colAssociatedWithIndex];

                // Update the hash index accordingly
                indexHash[entry].push_back(i);
            }
        }

        // Create the index based on the determined index type
        if (indextype == IndexType::BST)
        {
            // Create a new BST index
            // Iterate through each row in 'data'
            for (size_t i = 0; i < data.size(); ++i)
            {
                // Retrieve the corresponding value in 'colIndex' column for the current row
                TableEntry entry = data[i][colAssociatedWithIndex];

                // Update the BST index accordingly
                indexBST[entry].push_back(i);
            }
        }
    }
    
    std::cout << "Deleted " << numRowsDeleted << " rows from " << name << '\n';
}

void Table::generate()
{
    bool found;

    std::string indexWhich;
    std::cin >> indexWhich;

    std::string index;
    std::cin >> index;

    std::string on;
    std::cin >> on;

    std::string colName;
    std::cin >> colName;

    for (size_t j = 0; j < metaData.size(); ++j)
    {
        if (metaData[j].colName == colName)
        {
            colAssociatedWithIndex = j;
            found = true;
            break;
        }
    }

    //error if no such column name
    if (!found)
    {
        std::string junk;
        std::getline(std::cin, junk);
        std::cout << "Error during GENERATE: " << colName << " does not name a column in " << name << '\n';
        return;
    }

    indextype = (indexWhich == "hash") ? IndexType::HASH : IndexType::BST;

    // Check if an index already exists for the specified column
    if (indextype == IndexType::HASH && !indexHash.empty())
    {
        // Discard the old hash index
        indexHash.clear();
    }
    else if (indextype == IndexType::BST && !indexBST.empty())
    {
        // Discard the old BST index
        indexBST.clear();
    }

    // Create the index based on the determined index type
    if (indextype == IndexType::HASH)
    {
        // Create a new hash index
        // Iterate through each row in 'data'
        for (size_t i = 0; i < data.size(); ++i)
        {
            // Retrieve the corresponding value in 'colIndex' column for the current row
            TableEntry entry = data[i][colAssociatedWithIndex];

            // Update the hash index accordingly
            indexHash[entry].push_back(i);
        }

        // Print the success message
        std::cout << "Created hash index for table " << name << " on column " << metaData[colAssociatedWithIndex].colName;
        std::cout << ", with " << indexHash.size() << " distinct keys" << std::endl;
    }

    // Create the index based on the determined index type
    if (indextype == IndexType::BST)
    {
        // Create a new BST index
        // Iterate through each row in 'data'
        for (size_t i = 0; i < data.size(); ++i)
        {
            // Retrieve the corresponding value in 'colIndex' column for the current row
            TableEntry entry = data[i][colAssociatedWithIndex];

            // Update the BST index accordingly
            indexBST[entry].push_back(i);
        }

        // Print the success message
        std::cout << "Created bst index for table " << name << " on column " << metaData[colAssociatedWithIndex].colName;
        std::cout << ", with " << indexBST.size() << " distinct keys" << std::endl;
    }

}

void Table::join(const Table& table2, bool quiet)
{
    int numRowsPrinted = 0;

    bool found;

    size_t colIndex1 = 0;
    size_t colIndex2 = 0;

    std::unordered_map<TableEntry, std::vector<size_t>> indexTempHash;

    std::vector<std::pair<int, size_t>> tableColIndexPairs;

    std::string where;
    std::cin >> where;

    std::string colName1;
    std::cin >> colName1;

    char op;
    std::cin >> op;

    std::string colName2;
    std::cin >> colName2;

    std::string theWordAnd, print;
    std::cin >> theWordAnd;
    std::cin >> print;

    int numColsToPrint;
    std::cin >> numColsToPrint;

    size_t colIndex = 0;

    tableColIndexPairs.reserve(numColsToPrint);

    std::string colName;
    int whichTable;

    bool usingTempHash = false;

    for (int i = 0; i < numColsToPrint; ++i)
    {
        std::cin >> colName;
        std::cin >> whichTable;

        found = false;

        if (whichTable == 1)
        {
            for (size_t j = 0; j < metaData.size(); ++j)
            {
                if (metaData[j].colName == colName)
                {
                    colIndex = j;
                    found = true;
                    break;
                }
            }

            //error if no such column name
            if (!found)
            {
                std::string junk;
                std::getline(std::cin, junk);
                std::cout << "Error during JOIN: " << colName << " does not name a column in " << name << '\n';
                return;
            }
        }
        else // table 2
        {
            for (size_t j = 0; j < table2.metaData.size(); ++j)
            {
                if (table2.metaData[j].colName == colName)
                {
                    colIndex = j;
                    found = true;
                    break;
                }
            }

            //error if no such column name
            if (!found)
            {
                std::string junk;
                std::getline(std::cin, junk);
                //erroring here when it shouldn't
                std::cout << "Error during JOIN: " << colName << " does not name a column in " << table2.name << '\n';
                return;
            }
        }

        tableColIndexPairs.push_back({whichTable, colIndex});
    }

    if (!quiet)
    {
        for (int i = 0; i < numColsToPrint; ++i)
        {
            if (tableColIndexPairs[i].first == 1)
            {
                std::cout << metaData[tableColIndexPairs[i].second].colName << " ";
            }
            else
            {
                std::cout << table2.metaData[tableColIndexPairs[i].second].colName << " ";
            }
        }
        std::cout << '\n';
    }
    

    // check if column from table 1 is in table 1
    for (size_t j = 0; j < metaData.size(); ++j)
    {
        if (metaData[j].colName == colName1)
        {
            colIndex1 = j;
            found = true;
            break;
        }
    }

    //error if no such column name
    if (!found)
    {
        std::string junk;
        std::getline(std::cin, junk);
        std::cout << "Error during JOIN: " << colName1 << " does not name a column in " << name << '\n';
        return;
    }

    // check if column from table 2 is in table 2
    for (size_t j = 0; j < table2.metaData.size(); ++j)
    {
        if (table2.metaData[j].colName == colName2)
        {
            colIndex2 = j;
            found = true;
            break;
        }
    }

    //error if no such column name
    if (!found)
    {
        std::string junk;
        std::getline(std::cin, junk);
        std::cout << "Error during JOIN: " << colName2 << " does not name a column in " << table2.name << '\n';
        return;
    }

    // make a hash table for table 2 if there is not one already
    if (table2.indextype != IndexType::HASH || colIndex2 != table2.colAssociatedWithIndex )
    {
        usingTempHash = true;
        for (size_t i = 0; i < table2.data.size(); ++i)
        {
            // Retrieve the corresponding value in 'colIndex' column for the current row
            TableEntry entry = table2.data[i][colIndex2];

            // Update the hash index accordingly
            indexTempHash[entry].push_back(i);
        }
    }


    if (usingTempHash){
        for (size_t i = 0; i < data.size(); ++i)
        {

            TableEntry tableValueToCompare = data[i][colIndex1];

            // Use hash index
            auto it = indexTempHash.find(tableValueToCompare); 
                
            if (it == indexTempHash.end()) 
            {   
                continue;
            }
            

            for (size_t j : it->second)
            {
                ++numRowsPrinted;

                for (int k = 0; k < numColsToPrint; ++k)
                {
                    if (tableColIndexPairs[k].first == 1 && !quiet)
                    {
                        std::cout << data[i][tableColIndexPairs[k].second] << " ";
                    }
                    else if (!quiet)
                    {
                        std::cout << table2.data[j][tableColIndexPairs[k].second] << " ";
                    }
                }
                if (!quiet)
                    std::cout << '\n';
            }

        }
    }
    else
    {
        for (size_t i = 0; i < data.size(); ++i)
        {

            TableEntry tableValueToCompare = data[i][colIndex1];

            // Use hash index
            auto it = table2.indexHash.find(tableValueToCompare);
                
            if (it == table2.indexHash.end()) 
            {   
                continue;
            }
            

            for (size_t j : it->second)
            {
                ++numRowsPrinted;

                for (int k = 0; k < numColsToPrint; ++k)
                {
                    if (tableColIndexPairs[k].first == 1 && !quiet)
                    {
                        std::cout << data[i][tableColIndexPairs[k].second] << " ";
                    }
                    else if (!quiet)
                    {
                        std::cout << table2.data[j][tableColIndexPairs[k].second] << " ";
                    }
                }

                if (!quiet)
                    std::cout << '\n';
            }

        }
    }

    std::cout << "Printed " << numRowsPrinted << " rows from joining " << name << " to " << table2.name << '\n';

}
