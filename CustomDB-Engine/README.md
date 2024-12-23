# Silly QL: A Custom Database Engine

## Overview
Silly QL is a lightweight database engine that is built to explore the foundational concepts of database systems, including query parsing, data storage, and retrieval. This project helped me gain an understanding of how databases operate under the hood and reflects my ability to design efficient, modular systems.

TableEntry.h and TableEntry.cpp were provided as starter files, but all other work on this project is my own.

## **Skills Demonstrated**
- **Database Design**: Built a framework for SQL-like operations such as creating, inserting, printing, and joining tables.
- **Algorithm Development**: Implemented efficient query parsing, data storage, and retrieval mechanisms using C++.
- **System-Level Programming**: Managed data using in-memory structures and designed modular code for extensibility.

## **Key Features**
- **Command-Based Interface**: Supports commands like `CREATE`, `INSERT`, `PRINT`, `PRINTWHERE`, `DELETE`, `JOIN`, and more.
- **Error Handling**: Provides meaningful feedback for invalid operations or commands input by the user.
- **Efficient Data Management**: Tables are stored and managed using hash tables, enabling fast lookups, insertions, and deletions.

## **Example Usage**
```plaintext
% CREATE users
3
id name age
Created table users with 3 columns.

% INSERT INTO users
2 rows
1 Alice 25
2 Bob 30
Added 2 rows to users from position 0 to 1.

% PRINT users
2
id name
ALL
id: 1, name: Alice
id: 2, name: Bob

% REMOVE users
Table users removed.

% QUIT
Thanks for being silly!
