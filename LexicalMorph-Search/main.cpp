//IDENTIFIER  = 50EB44D3F029ED934858FFFCEAC3547C68768FC9

#include "p1-letterman.h"
#include <iostream>

int main(int argc, char *argv[]) {
  // This should be in all of your projects, speeds up I/O
  std::ios_base::sync_with_stdio(false);

  //declare letterman object
  Letterman letterman;

  //letterman object process command line
  Options options;
  letterman.getOptions(argc, argv, options);

  //std::cout << "here" << argc << argv[0] << std::endl;

  //letterman object read dictionary
  letterman.readDictionary(options);

  //letterman object do searching
  letterman.search(options);

  //maybe call output from search instead of calling from main
  //letterman object give output
  letterman.output(options);

  //letterman object return 0
  return 0;
}  
