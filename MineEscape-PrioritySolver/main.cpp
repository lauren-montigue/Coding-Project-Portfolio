// Project Identifier: 19034C8F3B1196BF8E0C6E1C0F973D2FD550B88F

#include <iostream>
#include "p2-mine-escape.h"

using namespace std;

int main(int argc, char *argv[]){

std::ios_base::sync_with_stdio(false);

//declare mineEscape object
mineEscape mineEscape;

//mineEscape option process command line
Options options;
//StatsOptions stats;
mineEscape.getOptions(argc, argv, options);

//mineEscape object process input
mineEscape.readInput();

//mineEscape object get out of mine
mineEscape.getOut(options);

//mineEscape object return 0
return 0;

}