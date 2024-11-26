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
            //std::cout << "bool" << std::endl;
        }
        if (colType == "string")
        {
            metaData.emplace_back("default_name", EntryType::String); //constructs and pushes back
            //std::cout << "string" << std::endl;
        }
        else if (colType == "int")
        {
            metaData.emplace_back("default_name", EntryType::Int); //constructs and pushes back
            //std::cout << "int" << std::endl;
        }
        else if (colType == "double")
        {
            metaData.emplace_back("default_name", EntryType::Double); //constructs and pushes back
            //std::cout << "double" << std::endl;
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
    //std::cout << "called table insert" << '\n';

    //std::cout << "data size at start is " << data.size() << std::endl;
    data.reserve(numRows + numRowsInserting);

    //std::cout << "total rows after insert should be " << numRows + numRowsInserting << std::endl;

     bool b;
     std::string s;
     int integer;
     double d;

     //std::cout << "num cols: " << numCols << std::endl;
     //std::cout << "num rows: " << numRows << std::endl;
     //std::cout << "num rows inserting: " << numRowsInserting << std::endl;

    for (int i = 0; i < numRowsInserting; ++i) //looping through rows being inserted
    {
        std::vector <TableEntry> tempRow;
        tempRow.reserve(numCols);
        //std::cout << "num cols is " << numCols << std::endl;

        for (int j = 0; j < numCols; ++j)
        {
            //already know the type because of guarantee that no type mismatches, check type saved in metadata
            if (metaData[j].colType == EntryType::Bool)
            {
               
                std::cin >> b;

                //std::cout << b << " ";

                TableEntry temp(b);
                tempRow.emplace_back(b);
            }
            else if (metaData[j].colType == EntryType::String)
            {
                std::cin >> s;

                //std::cout << s << " ";
                TableEntry temp(s);
                tempRow.emplace_back(s);
            }
            else if (metaData[j].colType == EntryType::Int)
            {
                
                std::cin >> integer;

                //std::cout << integer << " ";
                TableEntry temp(integer);
                tempRow.emplace_back(integer);
            }
            else 
            {
                
                std::cin >> d;

                //std::cout << d << " ";
                TableEntry temp(d);
                tempRow.emplace_back(d);
            }
        }
        // Append full row
        //std::cout << "size of temp row: " << tempRow.size() << std::endl;
        data.push_back(tempRow);
        //std::cout << "current num rows: " << data.size() << std::endl; 
        //std::cout << std::endl;
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
    //std::cout << "made it to printRow!" << std::endl;
    //std::cout << "indicesToPrint.size(): " << indicesToPrint.size() << std::endl;
    //std::cout << "currentRowIdx = " << currentRowIdx << std::endl;
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
    //std::cout << "made it to table printAll!" << std::endl;
    //std::cout << "listOfColNames.size() = " << listOfColNames.size() << std::endl;
    //std::cout << "indicesToPrint.size() = " << indicesToPrint.size() << std::endl;

    if (!quiet)
    {
        for (size_t i = 0; i < listOfColNames.size(); ++i)
        {
            std::cout << listOfColNames[i] << " ";
        }

        std::cout << '\n';
    }
    

    //std::cout << "hello" << std::endl;

    //std::cout << "num rows: " << numRows << std::endl;

    for (int row = 0; row < numRows; ++row)
    {
        //std::cout << "hi there" << std::endl;
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

    //std::cout << "how many cols in meta data? " << metaData.size() << std::endl;

    size_t colIndex = 0;
    bool found;

    for (size_t j = 0; j < metaData.size(); ++j)
    {
        //std::cout << "col name: " << metaData[j].colName << std::endl;
        if (metaData[j].colName == colName)
        {
            //std::cout << "okay yeah found the column " << std::endl;
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
            //std::cout << "using bst index for bool" << std::endl;
            // Use BST index
            if (op == ">")
            {
                //std::cout << "calling greater than" << std::endl;
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
                //std::cout << "calling less than" << std::endl;
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
                //std::cout << "calling equal to" << std::endl;
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
            //std::cout << "using bst index for string" << std::endl;
            if (op == ">")
            {
                //std::cout << "calling greater than" << std::endl;
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
                //std::cout << "calling less than" << std::endl;
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
                //std::cout << "calling equal to" << std::endl;
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
            //std::cout << "using bst index for int" << std::endl;
            if (op == ">")
            {
                //std::cout << "calling greater than" << std::endl;
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
                //std::cout << "calling less than" << std::endl;
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
                //std::cout << "calling equal to" << std::endl;
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
            //std::cout << "using bst index for double" << std::endl;

            if (op == ">")
            {
                //std::cout << "calling greater than" << std::endl;
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
                //std::cout << "calling less than" << std::endl;
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
                //std::cout << "calling  equal to" << std::endl;
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
    //std::cout << "made it to table join" << std::endl;
    int numRowsPrinted = 0;

    bool found;

    size_t colIndex1 = 0;
    size_t colIndex2 = 0;

    std::unordered_map<TableEntry, std::vector<size_t>> indexTempHash;

    std::vector<std::pair<int, size_t>> tableColIndexPairs;

    std::string where;
    std::cin >> where;

    //std::cout << "where: " << where << std::endl;

    std::string colName1;
    std::cin >> colName1;

    //std::cout << "colName1 " << colName1 << std::endl;

    char op;
    std::cin >> op;

    std::string colName2;
    std::cin >> colName2;

    //std::cout << "colName2 " << colName2 << std::endl;

    std::string theWordAnd, print;
    std::cin >> theWordAnd;
    std::cin >> print;

    int numColsToPrint;
    std::cin >> numColsToPrint;

    //std::cout << "numColsToPrint: " << numColsToPrint << std::endl;

    size_t colIndex = 0;

    tableColIndexPairs.reserve(numColsToPrint);

    std::string colName;
    int whichTable;

    bool usingTempHash = false;

    for (int i = 0; i < numColsToPrint; ++i)
    {
        std::cin >> colName;
        std::cin >> whichTable;

        //std::cout << "colName: " << colName << std::endl;
        //std::cout << "whichTable: " << whichTable << std::endl;

        found = false;

        if (whichTable == 1)
        {
            for (size_t j = 0; j < metaData.size(); ++j)
            {
                //std::cout << "col name: " << metaData[j].colName << std::endl;
                if (metaData[j].colName == colName)
                {
                    //std::cout << "okay yeah found the column " << std::endl;
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
                //std::cout << "col name: " << metaData[j].colName << std::endl;
                if (table2.metaData[j].colName == colName)
                {
                    //std::cout << "okay yeah found the column " << std::endl;
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
        //std::cout << "pushing back " << whichTable << " " << colIndex << std::endl;
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
        //std::cout << "col name: " << metaData[j].colName << std::endl;
        if (metaData[j].colName == colName1)
        {
            //std::cout << "okay yeah found the column " << std::endl;
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
        //std::cout << "col name: " << metaData[j].colName << std::endl;
        if (table2.metaData[j].colName == colName2)
        {
            //std::cout << "okay yeah found the column " << std::endl;
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
        //std::cout << "creating hash table for " << table2.name << std::endl;
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
            //std::cout << "looping through row for " << name << std::endl;

            TableEntry tableValueToCompare = data[i][colIndex1];
            //std::cout << data[i][colIndex1] << std::endl;

            // Use hash index
            auto it = indexTempHash.find(tableValueToCompare); 
                
            if (it == indexTempHash.end()) 
            {   
                //std::cout << "found no matches sorry :(" << std::endl;
                continue;
            }
            

            for (size_t j : it->second)
            {
                ++numRowsPrinted;
                //std::cout << "looping through row for " << table2.name << std::endl;

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
            //std::cout << "looping through row for " << name << std::endl;

            TableEntry tableValueToCompare = data[i][colIndex1];
            //std::cout << data[i][colIndex1] << std::endl;

            // Use hash index
            auto it = table2.indexHash.find(tableValueToCompare);
                
            if (it == table2.indexHash.end()) 
            {   
                //std::cout << "found no matches sorry :(" << std::endl;
                continue;
            }
            

            for (size_t j : it->second)
            {
                ++numRowsPrinted;
                //std::cout << "looping through row for " << table2.name << std::endl;

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

//INDICES

//equality checks - hash index
//< > ordered map (bst)

//can use std:: lower bound, upper bound 
//given an ordered map, can tell yo uin faster than linear time which index you need to loop to

//generate makes the index that you can use in delete, join, printwhere

//order to code:
//write generate
//then printwhere with index
//break down into different operations
//did you already generate a bst index?
//if yes, then use. if no, then have to do brute force search (which you've already coded)

//join tips:
//prints everything that matches in those tables (equality check)
//speed up : dont do O(n^2), go through all first table and if no hash index exists for second table, create temp hash index for second table

//keep code structured, lots of if statements - think about helper functions

//goal for friday: write printwhere, generate, delte

//join is kind of its own thign

//use all autograder submissions
