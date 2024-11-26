//IDENTIFIER  = 50EB44D3F029ED934858FFFCEAC3547C68768FC9

// EECS 281, Project 1
#include "p1-letterman.h"

#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <deque>

using namespace std;

//Private member function implementation

    bool Letterman::change(const size_t& currentWordIdx, const size_t& dictionaryWordIdx)
    {
      //cout << "change is called" << endl;

        if (dictEntries[dictionaryWordIdx].word.length() == dictEntries[currentWordIdx].word.length()
         && (!dictEntries[dictionaryWordIdx].beenDiscovered || currentlyBacktracking))
        {
          int numDifferentLetters = 0;
          //go through letter by letter and see if only one is different
          for (size_t j = 0; j < dictEntries[dictionaryWordIdx].word.length(); ++j)
          {
            if (dictEntries[dictionaryWordIdx].word[j] != dictEntries[currentWordIdx].word[j]) 
              ++numDifferentLetters;
          }
          //if only one different letter, push it onto the deque
          if (numDifferentLetters == 1)
          {
            return true;
          }
        }
        
        return false;
    }

    bool Letterman::swap(const size_t &currentWordIdx, const size_t &dictionaryWordIdx)
    {
      int numPossibleSwaps = 0;

        if (dictEntries[dictionaryWordIdx].word.length() == dictEntries[currentWordIdx].word.length()
        && (!dictEntries[dictionaryWordIdx].beenDiscovered || currentlyBacktracking))
        {
          bool lastLetterChecked = false;
          //go through letter by letter and see if two can be swapped to form a different dictionary word
          for (size_t j = 0; j < dictEntries[dictionaryWordIdx].word.length() - 1; ++j)
          {
            //if the letters at the same position are different
            if (dictEntries[dictionaryWordIdx].word[j] != dictEntries[currentWordIdx].word[j]){ //QUESTION: correct syntax for this?
              
              //if the following letter is NOT a valid swap
              if (dictEntries[dictionaryWordIdx].word[j + 1] != dictEntries[currentWordIdx].word[j] 
              || dictEntries[dictionaryWordIdx].word[j] != dictEntries[currentWordIdx].word[j + 1]){ //QUESTION: correct syntax for this?
                return false;
              }
              else{
                ++numPossibleSwaps;
                ++j;
                //in case the last two letters are the swap
                if (j == dictEntries[dictionaryWordIdx].word.length() - 1)
                  lastLetterChecked = true;
              }
            }
          }
          //if statement for if last letter is different and it wasnt alr checked w swap
          if (!lastLetterChecked && 
              dictEntries[dictionaryWordIdx].word[dictEntries[dictionaryWordIdx].word.length() - 1] != dictEntries[currentWordIdx].word[dictEntries[currentWordIdx].word.length() - 1])
              return false;
        }

        if (numPossibleSwaps == 1)
        {
          return true;
        }

        return false;
    }

    bool Letterman::insert(const size_t &currentWordIdx, const size_t &dictionaryWordIdx)
    {
      for (size_t j = 0; j < dictEntries[currentWordIdx].word.length(); ++j)
      {
        if (dictEntries[dictionaryWordIdx].word[j] != dictEntries[currentWordIdx].word[j])
        {
          return(dictEntries[currentWordIdx].word.substr(j) == dictEntries[dictionaryWordIdx].word.substr(j + 1));
        }
      } //for loop - parse through all letters and compare for each word
      
      //if the loop never reaches a different letter, then the only different letter is the last one and an insert is possible
      return true;
    }

    bool Letterman::length(const size_t &currentWordIdx, const size_t &dictionaryWordIdx)
    {
      //check if possible insert valid
      if (dictEntries[dictionaryWordIdx].word.length() == dictEntries[currentWordIdx].word.length() + 1
      && (!dictEntries[dictionaryWordIdx].beenDiscovered || currentlyBacktracking))
      {
        return insert(currentWordIdx, dictionaryWordIdx);
      } //if length is such that insert might be possible

      //check if possible delete valid - the opposite of insert
      else if (dictEntries[dictionaryWordIdx].word.length() == dictEntries[currentWordIdx].word.length() - 1
      && (!dictEntries[dictionaryWordIdx].beenDiscovered || currentlyBacktracking))
      {
        //cout << "delete may be valid for " << dictEntries[currentWordIdx].word << " to " << dictEntries[dictionaryWordIdx].word << endl;
        return insert(dictionaryWordIdx, currentWordIdx);
      } // if length is such that delete might be possible
      
      return false;
    }

  //Public member functions implementation
    Letterman::Letterman() {    }

    //TODO print helpful message
    void Letterman::printHelp(char *argv[]) {
      cout << "Usage: " << argv[0] << " [-m resize|reserve|nosize] | -h\n";
      cout << "This program is to help you learn command-line processing,\n";
      cout << "reading data into a vector, the difference between resize and\n";
      cout << "reserve and how to properly read until end-of-file." << endl;
    }  // printHelp()

    void Letterman::getOptions(int argc, char * argv[], Options &options) {
    // These are used with getopt_long()
    opterr = false; // Let us handle all error output for command line options
    int choice;
    int index = 0;
    option long_options[] = {
      { "help", no_argument, nullptr, 'h'},
      { "queue", no_argument, nullptr, 'q'},
      { "stack", no_argument, nullptr, 's'},
      { "begin", required_argument, nullptr, 'b'},
      { "end", required_argument, nullptr, 'e'},
      { "output", required_argument, nullptr, 'o'},
      { "change", no_argument, nullptr, 'c'},
      { "length", no_argument, nullptr, 'l'},
      { "swap", no_argument, nullptr, 'p'},
      { nullptr, 0, nullptr, '\0' },
    };  // long_options[]

    //parse through getopt_long, make changes based on switch blocks

    while ((choice = getopt_long(argc, argv, "hqsb:e:o:clw", long_options, &index)) != -1) { //index needed, counts arguments

      string arg = "";
      switch (choice) { //choice represents user input to command line
        case 'h':
        {
          printHelp(argv);
          exit(0);
        }

        case 'q': 
        {
          if (options.stackOrQueueWhich != stackOrQueue::kNone)
          {
            cerr << "Error: Must choose only one out of queue and stack modes" << endl;
            exit(1);
          }
          options.stackOrQueueWhich = stackOrQueue::kIsQueue;
          break;
        }
        
        case 's':
        {
          if (options.stackOrQueueWhich != stackOrQueue::kNone)
          {
            cerr << "Error: Must choose only one out of queue and stack modes, not both" << endl;
            exit(1);
          }
          options.stackOrQueueWhich = stackOrQueue::kIsStack;
          break;
        }

        case 'b':
        {
          arg = optarg;
          options.beginWord = arg;
          break;
        }

        case 'e':
        {
          arg = optarg;
          options.endWord = arg;
          break;
        }

        case 'o':
        {
          arg = optarg;
          if (arg != "W" && arg != "M" && arg != "")
          {
            cerr << "Error: invalid string. Must choose either word mode (W) or modification mode (M)" << endl;
            exit(1);
          }
          else if (arg == "W")
          {
            options.wordOrMorphWhich = wordOrMorph::kWordMode;
          }
          else
          {
            options.wordOrMorphWhich = wordOrMorph::kMorphMode;
          }
          break;
        }

        case 'c':
        {
          changeFlag = true;
          options.allowedModificationsWhich.push_back(allowedModifications::kChangeAllowed);
          break;
        }

        case 'l':
        {
          lengthFlag = true;
          options.allowedModificationsWhich.push_back(allowedModifications::kLengthAllowed);
          break;
        }

        case 'p':
        {
          swapFlag = true;
          options.allowedModificationsWhich.push_back(allowedModifications::kSwapAllowed);
          break;
        }

        case '?':
        {
          swapFlag = true;
          options.allowedModificationsWhich.push_back(allowedModifications::kSwapAllowed);
          break;
        }

        default:
        {
          cerr << "Error: invalid option" << endl;
          exit(1); //covers the case of no argument
        }
      }  // switch ..choice

      

    }  // while

      // Check if none of --change, --length, or --swap options were provided
      if (!changeFlag && !lengthFlag && !swapFlag) {
          cerr << "Error: You must specify at least one of --change, --length, or --swap" << endl;
          exit(1);
      }

      // check if none of --stack or --queue options were provided
      if (options.stackOrQueueWhich == stackOrQueue::kNone) {
        cerr << "Error: You must specify one of --stack or --queue" << endl;
        exit(1);
      }
  }  // if ..mode

  void Letterman::readDictionary(const Options &options){
    string input;
    dictionaryEntry tempWord; 

      std::getline(std::cin, input);
      if (input == "S")
      {
        std::getline(std::cin, input);
        size_t simpleDictSize = std::stoul(input); 
        dictEntries.reserve(simpleDictSize);

        while (std::getline(std::cin, input))
        {
          if (input[0] != '/' && (lengthFlag == 1 || input.length() == options.beginWord.length()))
          {
            tempWord.word = input;
            dictEntries.push_back(tempWord);
            
            if (tempWord.word == options.beginWord)
            {
              beginWordIdx = dictEntries.size() - 1; 
              searchContainer.push_back(beginWordIdx);
              dictEntries[beginWordIdx].beenDiscovered = true;
              ++numWordsDiscovered;
              beginWordInDictionary = true;
            }
            if (tempWord.word == options.endWord)
            {
              endWordIdx = dictEntries.size() - 1;
              endWordInDictionary = true;
            }
          }
        }
        
      } //if simple

      else 
      {
        std::getline(std::cin, input);

        size_t pos; 

        while (std::getline(std::cin, input))
        {
          if (input[0] != '/')
          {
            
            //reversal
            if (!input.empty() && input.find('&') != std::string::npos
                 && (lengthFlag == 1 || input.length() == options.beginWord.length() + 1))
            {
              std::string forwardsWord = input;
              //erase ampersand, you already know exact position
              forwardsWord.erase(forwardsWord.length() - 1, 1);
              string backwardsWord = "";

              for (size_t i = forwardsWord.length(); i > 0; --i) 
              {
                backwardsWord.push_back(forwardsWord[i - 1]);
              }

              tempWord.word = forwardsWord; 
              dictEntries.push_back(tempWord);

              if (tempWord.word == options.beginWord)
              {
                beginWordIdx = dictEntries.size() - 1;
                searchContainer.push_back(beginWordIdx);
                dictEntries[beginWordIdx].beenDiscovered = true;
                ++numWordsDiscovered;
                beginWordInDictionary = true;
              }
              if (tempWord.word == options.endWord)
              {
                endWordIdx = dictEntries.size() - 1;
                endWordInDictionary = true;
              }

              if (forwardsWord != backwardsWord)
              {
                tempWord.word = backwardsWord;
                dictEntries.push_back(tempWord);

                if (tempWord.word == options.beginWord)
                {
                  beginWordIdx = dictEntries.size() - 1;
                  searchContainer.push_back(beginWordIdx);
                  dictEntries[beginWordIdx].beenDiscovered = true;
                  ++numWordsDiscovered;
                  beginWordInDictionary = true;
                }
                if (tempWord.word == options.endWord)
                {
                  endWordIdx = dictEntries.size() - 1;
                  endWordInDictionary = true;
                }
              }
            }

            //double
            else if (!input.empty() && input.find('?') != std::string::npos
                      && (lengthFlag == 1 || input.length() == options.beginWord.length() + 1 || input.length() == options.beginWord.length()))
            {
              pos = input.find('?');

              string singleLetter = input;
              string doubleLetter = input;

              singleLetter.erase(pos,1);
              doubleLetter.replace(pos,1,1,doubleLetter[pos - 1]);

              tempWord.word = singleLetter;
              dictEntries.push_back(tempWord);

              if (tempWord.word == options.beginWord)
              {
                beginWordIdx = dictEntries.size() - 1;
                searchContainer.push_back(beginWordIdx);
                dictEntries[beginWordIdx].beenDiscovered = true;
                ++numWordsDiscovered;
                beginWordInDictionary = true;
              }
              if (tempWord.word == options.endWord)
              {
                endWordIdx = dictEntries.size() - 1;
                endWordInDictionary = true;
              }

              tempWord.word = doubleLetter;
              dictEntries.push_back(tempWord);

              if (tempWord.word == options.beginWord)
              {
                beginWordIdx = dictEntries.size() - 1;
                searchContainer.push_back(beginWordIdx);
                dictEntries[beginWordIdx].beenDiscovered = true;
                ++numWordsDiscovered;
                beginWordInDictionary = true;
              }
              if (tempWord.word == options.endWord)
              {
                endWordIdx = dictEntries.size() - 1;
                endWordInDictionary = true;
              }
            }

            //swap
            else if (!input.empty() && input.find('!') != std::string::npos
                    && (lengthFlag == 1 || input.length() == options.beginWord.length() + 1))
            {
              pos = input.find('!');

              string swapped = input;
              string regular = input;

              //erase ! in both words
              regular.erase(pos, 1);
              swapped.erase(pos, 1);

              //use regular to swap for swapped
              swapped.replace(pos - 2, 1, 1,regular[pos - 1]);
              swapped.replace(pos - 1, 1, 1,regular[pos - 2]);

              tempWord.word = regular;
              dictEntries.push_back(tempWord);

              if (tempWord.word == options.beginWord)
              {
                beginWordIdx = dictEntries.size() - 1;
                searchContainer.push_back(beginWordIdx);
                dictEntries[beginWordIdx].beenDiscovered = true;
                ++numWordsDiscovered;
                beginWordInDictionary = true;
              }
              if (tempWord.word == options.endWord)
              {
                endWordIdx = dictEntries.size() - 1;
                endWordInDictionary = true;
              }

              tempWord.word = swapped;
              dictEntries.push_back(tempWord);

              if (tempWord.word == options.beginWord)
              {
                beginWordIdx = dictEntries.size() - 1;
                searchContainer.push_back(beginWordIdx);
                dictEntries[beginWordIdx].beenDiscovered = true;
                ++numWordsDiscovered;
                beginWordInDictionary = true;
              }
              if (tempWord.word == options.endWord)
              {
                endWordIdx = dictEntries.size() - 1;
                endWordInDictionary = true;
              }
            }

            //insert each
            else if (!input.empty() && input.find('[') != std::string::npos) 
            {
              size_t openBracketPos = input.find('[');
              size_t closeBracketPos = input.find(']');

              if (lengthFlag == 1 || input.length() == options.beginWord.length() + closeBracketPos - openBracketPos)
              {
                for (size_t i = 1; i < closeBracketPos - openBracketPos; ++i)
                {
                  string tempString = "";

                  //append beginning before open bracket
                  tempString.append(input.substr(0,openBracketPos));

                  //append next letter
                  tempString.append(input.substr(openBracketPos + i, 1));
                  
                  //append substring after closed bracket
                  tempString.append(input.substr(closeBracketPos + 1));

                  tempWord.word = tempString;
                  dictEntries.push_back(tempWord);

                  if (tempWord.word == options.beginWord)
                  {
                    beginWordIdx = dictEntries.size() - 1;
                    searchContainer.push_back(beginWordIdx);
                    dictEntries[beginWordIdx].beenDiscovered = true;
                    ++numWordsDiscovered;
                    beginWordInDictionary = true;
                  }
                  if (tempWord.word == options.endWord)
                  {
                    endWordIdx = dictEntries.size() - 1;
                    endWordInDictionary = true;
                  }
                    
                } 
              }          
          }

          //just plain word
          else if (lengthFlag == 1 || input.length() == options.beginWord.length())
          {
            tempWord.word = input;
            dictEntries.push_back(tempWord);

            if (tempWord.word == options.beginWord)
            {
              beginWordIdx = dictEntries.size() - 1;
              searchContainer.push_back(beginWordIdx);
              dictEntries[beginWordIdx].beenDiscovered = true;
              ++numWordsDiscovered;
              beginWordInDictionary = true;
            }
            if (tempWord.word == options.endWord)
            {
              endWordIdx = dictEntries.size() - 1;
              endWordInDictionary = true;
            }
          }
        } //while get

        } // while parsing through
      } // if complex

      if (!(beginWordInDictionary && endWordInDictionary))
      {
        cerr << "Error: Both begin and end words must be in dictionary" << endl;
        exit(1);
      }

      //if length not specified and begin and end words are different lengths
      auto itLength = std::find(options.allowedModificationsWhich.begin(), options.allowedModificationsWhich.end(), allowedModifications::kLengthAllowed);
      if (itLength == options.allowedModificationsWhich.end() && options.beginWord.length() != options.endWord.length())
      {
        cerr << "Error: Begin and end words are different lengths, but length modification is not allowed" << endl;
        exit(1);
      }
    }// readDictionary
    

    void Letterman::search(const Options &options)
    {
      size_t currentIdx; 

      if (options.beginWord == options.endWord)
      {
        reachedEndWord = true;
      }

      //nested loop
      while (!searchContainer.empty() && !reachedEndWord) 
      {
        int currentIdxInt;

        if (options.stackOrQueueWhich == stackOrQueue::kIsStack)
        {
          currentIdx = searchContainer.back();
          searchContainer.pop_back(); 
        }
        else
        {
          currentIdx = searchContainer.front();
          searchContainer.pop_front(); 
        }

        //convert size_t to int
        currentIdxInt = static_cast<int>(currentIdx);

        for (size_t i = 0; i < dictEntries.size(); ++i)
        {
          if (!dictEntries[i].beenDiscovered)
          {
            auto itChange = std::find(options.allowedModificationsWhich.begin(), options.allowedModificationsWhich.end(), allowedModifications::kChangeAllowed);
            auto itSwap = std::find(options.allowedModificationsWhich.begin(), options.allowedModificationsWhich.end(), allowedModifications::kSwapAllowed);
            auto itLength = std::find(options.allowedModificationsWhich.begin(), options.allowedModificationsWhich.end(), allowedModifications::kLengthAllowed);

            //if change is allowed and possible
            if (itChange != options.allowedModificationsWhich.end() && change(currentIdx, i)) 
            {
              searchContainer.push_back(i);
              ++numWordsDiscovered;
              dictEntries[i].prev = currentIdxInt;
              dictEntries[i].beenDiscovered = true;

              if (i == endWordIdx)
              {
                reachedEndWord = true;
                break; 
              }
            }
            else if (itSwap != options.allowedModificationsWhich.end() && swap(currentIdx, i))
            {
              searchContainer.push_back(i);
              ++numWordsDiscovered;
              dictEntries[i].prev = currentIdxInt;
              dictEntries[i].beenDiscovered = true;

              if (i == endWordIdx)
              {
                reachedEndWord = true;
                break; 
              }
            }
            else if (itLength != options.allowedModificationsWhich.end() && length(currentIdx, i))
            {
              searchContainer.push_back(i);
              ++numWordsDiscovered;
              dictEntries[i].prev = currentIdxInt;
              dictEntries[i].beenDiscovered = true;

              if (i == endWordIdx)
              {
                reachedEndWord = true;
                break; 
              }
            }
          }
        }
      } // while searchContainer is not empty and we have not reached the last word
      
    }

    void Letterman::output(const Options &options)
    {
      currentlyBacktracking = true;

      if (!reachedEndWord)
      {
        cout << "No solution, " << numWordsDiscovered << " words discovered." << endl;
      }

      else if (options.beginWord == options.endWord)
      {
        cout << "Words in morph: 1" << endl;
        cout << options.beginWord << endl;
      }

      //word mode
      else if (options.wordOrMorphWhich == wordOrMorph::kWordMode || options.wordOrMorphWhich == wordOrMorph::kNone)
      {
        //implement deque that functions as stack
        deque <int> pathToEndWord;
        int i = static_cast<int>(endWordIdx);
        int wordsInMorph = 0;

        while (i != -1)
        {
          pathToEndWord.push_back(i);
          i = dictEntries[i].prev;
          ++wordsInMorph;
        }

        cout << "Words in morph: " << wordsInMorph << endl;

        while (!pathToEndWord.empty()) 
        {
          cout << dictEntries[pathToEndWord[pathToEndWord.size() - 1]].word << endl;
          pathToEndWord.pop_back();
        }
      } // if wordMode

      //if morph / modification mode
      else
      {
        deque <char> pathToEndWord;
        pathToEndWord.push_back('\n');
        int i = static_cast<int>(endWordIdx);
        int wordsInMorph = 1;

        char jChar;

        while (dictEntries[i].prev != -1)
        {
          //check i and its prev - how did it get there?
          //check in same order - change, length, swap

          if (change(dictEntries[i].prev, i))
          {
            //valid change from the word before to this word
            for (size_t j = 0; j < dictEntries[i].word.length(); ++j)
            {
              //if you reach the different letter while parsing through
              if (dictEntries[i].word[j] != dictEntries[dictEntries[i].prev].word[j])
              {
                pathToEndWord.push_back(dictEntries[i].word[j]);

                pathToEndWord.push_back(',');

                //push back position
                if (j < 10) {
                    jChar = static_cast<char> (j + 48);
                    pathToEndWord.push_back(jChar);
                }
                else {
                    int tens = static_cast<int>(j)/ 10; // Get the tens digit
                    int ones = static_cast<int>(j) % 10; // Get the ones digit
                    pathToEndWord.push_back(static_cast<char>(ones + 48)); // Convert ones digit to ASCII character
                    pathToEndWord.push_back(static_cast<char>(tens + 48)); // Convert tens digit to ASCII character
                }

                pathToEndWord.push_back(',');
              }
              
            } //for loop, parse through each letter of this word and the previous

            pathToEndWord.push_back('c');
            
          }

          else if (swap(dictEntries[i].prev, i))
          {
            //valid swap from the word before to this word

            for (size_t j = 0; j < dictEntries[i].word.length(); ++j)
            {
              //if you reach the different letter while parsing through
              if (dictEntries[i].word[j] != dictEntries[dictEntries[i].prev].word[j])
              {
                //then swap starts at this letter

                if (j < 10) {
                    jChar = static_cast<char> (j + 48);
                    pathToEndWord.push_back(jChar);
                }
                else {
                    int tens = static_cast<int>(j)/ 10; // Get the tens digit
                    int ones = static_cast<int>(j) % 10; // Get the ones digit
                    pathToEndWord.push_back(static_cast<char>(ones + 48)); // Convert ones digit to ASCII character
                    pathToEndWord.push_back(static_cast<char>(tens + 48)); // Convert tens digit to ASCII character
                }

                pathToEndWord.push_back(',');

                break;
              }
            }

            pathToEndWord.push_back('s');

          }

          else
          {
            //valid insert or delete from the word before to this word      
            //if insert valid
            if (dictEntries[dictEntries[i].prev].word.length() < dictEntries[i].word.length())
            {              
              bool foundLetter = false;

              //looping through the length of the previous word before the insert happened
              for (size_t j = 0; j < dictEntries[dictEntries[i].prev].word.length(); ++j)
              {
                if (dictEntries[i].word[j] != dictEntries[dictEntries[i].prev].word[j] && !foundLetter)
                {
                  foundLetter = true;

                  pathToEndWord.push_back(dictEntries[i].word[j]);

                  pathToEndWord.push_back(',');
                  
                  //push back position
                  if (j < 10) {
                      jChar = static_cast<char> (j + 48);
                      pathToEndWord.push_back(jChar);
                  }
                  else {
                      int tens = static_cast<int>(j)/ 10; // Get the tens digit
                      int ones = static_cast<int>(j) % 10; // Get the ones digit
                      pathToEndWord.push_back(static_cast<char>(ones + 48)); // Convert ones digit to ASCII character
                      pathToEndWord.push_back(static_cast<char>(tens + 48)); // Convert tens digit to ASCII character
                  }

                  pathToEndWord.push_back(',');
                }
              }

              if (!foundLetter)
              {
                //push back the last letter of updated word
                pathToEndWord.push_back(dictEntries[i].word[dictEntries[i].word.length() - 1]);

                pathToEndWord.push_back(',');
                size_t indexLastLetter = dictEntries[i].word.length() - 1;
                
                //push back position
                if (indexLastLetter < 10) {
                    jChar = static_cast<char> (indexLastLetter + 48);
                    pathToEndWord.push_back(jChar);
                }
                else {
                    int tens = static_cast<int>(indexLastLetter)/ 10; // Get the tens digit
                    int ones = static_cast<int>(indexLastLetter) % 10; // Get the ones digit
                    pathToEndWord.push_back(static_cast<char>(ones + 48)); // Convert ones digit to ASCII character
                    pathToEndWord.push_back(static_cast<char>(tens + 48)); // Convert tens digit to ASCII character
                }
                
                pathToEndWord.push_back(',');
              }

              pathToEndWord.push_back('i');
            }

            //if delete valid
            else
            {
              bool foundLetter = false;

              //looping through the length of the updated word after delete happened
              for (size_t j = 0; j < dictEntries[i].word.length(); ++j)
              {
                
                if (dictEntries[i].word[j] != dictEntries[dictEntries[i].prev].word[j] && !foundLetter)
                {
                  foundLetter = true;

                  //push back the position of deletion
                  if (j < 10) {
                      jChar = static_cast<char> (j + 48);
                      pathToEndWord.push_back(jChar);
                  }
                  else {
                      int tens = static_cast<int>(j)/ 10; // Get the tens digit
                      int ones = static_cast<int>(j) % 10; // Get the ones digit
                      pathToEndWord.push_back(static_cast<char>(ones + 48)); // Convert ones digit to ASCII character
                      pathToEndWord.push_back(static_cast<char>(tens + 48)); // Convert tens digit to ASCII character
                  }

                  pathToEndWord.push_back(',');
                }
              }

              if (!foundLetter)
              {
                //push back the position of deletion, which is the size of updated word
                size_t indexLastLetter = dictEntries[i].word.length();

                if (indexLastLetter < 10) {
                    jChar = static_cast<char> (indexLastLetter + 48);
                    pathToEndWord.push_back(jChar);
                }
                else {
                    int tens = static_cast<int>(indexLastLetter)/ 10; // Get the tens digit
                    int ones = static_cast<int>(indexLastLetter) % 10; // Get the ones digit
                    pathToEndWord.push_back(static_cast<char>(ones + 48)); // Convert ones digit to ASCII character
                    pathToEndWord.push_back(static_cast<char>(tens + 48)); // Convert tens digit to ASCII character
                }

                pathToEndWord.push_back(',');
              }

              pathToEndWord.push_back('d');
            }
          }

          if (dictEntries[dictEntries[i].prev].prev != -1)
              pathToEndWord.push_back('\n');

          i = dictEntries[i].prev;
          ++wordsInMorph;
        }

        cout << "Words in morph: " << wordsInMorph << endl;
        cout << options.beginWord << endl;

        while (!pathToEndWord.empty()) 
        {
          cout << pathToEndWord[pathToEndWord.size() - 1];
          pathToEndWord.pop_back();
        }


      } //morph mode

    } //output