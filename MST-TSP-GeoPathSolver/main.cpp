// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0

#include "drone.h"
#include <iostream>

int main(int argc, char *argv[]) {
  // This should be in all of your projects, speeds up I/O
  std::ios_base::sync_with_stdio(false);

  //declare drone object
  Drone drone;

  //letterman object process command line
  Options options;
  drone.getOptions(argc, argv, options);

  drone.readInput(options);

  //letterman object return 0
  return 0;
}  
