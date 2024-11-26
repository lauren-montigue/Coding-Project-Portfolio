// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include "p3-sillyql.h"
#include <iostream>

int main(int argc, char *argv[]) {
  // This should be in all of your projects, speeds up I/O
  std::ios_base::sync_with_stdio(false);

  std::cin >> std::boolalpha;
  std::cout << std::boolalpha;

  //declare SillyQL object
  SillyQL silly;

  silly.getOptions(argc, argv);

  //SillyQL object read input
  silly.readInput();

  //letterman object return 0
  return 0;
}  

/*{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": []
}*/
