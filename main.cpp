#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include <map>
#include <utility>

using namespace std;

string removeJunk(string& word);
string validation(string& potentialWord);
string lowerCase(string& Validword);
bool isAlpha(string& word);
void printA(pair<string, int> mapPair);
void printF(pair<string, int> mapPair);
void printO(vector<string>& cleanedWords, int& n);

int main(int argc, char **argv)
{

  // Exception Handling
  string file{};
  string sort{};
  int n{};

  try
  {
    switch(argc)
    {
      case 1:
      {
        cerr << "Error. No arguments given." << endl;
        return -1;
      }
      case 2:
      {
        cerr << "Error: Second argument missing or invalid." << endl;
        return -1;
      }
      case 3:
      {
        file = argv[1];
        sort = argv[2];
        break;
      }
      case 4:
      {
        file = argv[1];
        sort = argv[2];
        n = stoi(argv[3]);
        break;
      }
      default:
      {
        cerr << "Error: Argument(s) missing or invalid." << endl;
      }
    }
  }
  catch(invalid_argument& k)
  {
    cerr << k.what() << endl;
    return -1;
  }
  catch(logic_error& e)
  {
    cerr << "Usage: ./a.out FILE [-a] [-f] [-o N]" << endl;
    return -1;
  }


int newSize{};
string space{" "};
vector<string> words;
map<string, int> myMap;
fstream myFile{file};
istream_iterator<string> iis{myFile};
istream_iterator<string> eos;

// Copying words from file to vector using iterator
copy(iis, eos, back_inserter(words));

// Word collection and washing
transform(words.begin(), words.end(), words.begin(), removeJunk);
transform(words.begin(), words.end(), words.begin(), validation);
transform(words.begin(), words.end(), words.begin(), lowerCase);

// Copying to new vector after removing spaces
newSize = count(words.begin(), words.end(),space);
vector<string>cleanedWords(words.size()-newSize);
copy_if(words.begin(), words.end(), cleanedWords.begin(), [space](string word){return !(word == space);});

// Inserting to map
for_each(cleanedWords.begin(),cleanedWords.end(), [&myMap](string word){myMap[word]++;});


// Sorting and Printing
cout << setfill(' ') << setw(10) << "KEY" << setfill(' ') << setw(10) << "VALUE" << endl << endl;

if(sort == "-a")
{
  for_each(myMap.begin(), myMap.end(), printA);
}
else if(sort == "-f")
{
  vector<pair<string, int>> fSortWords;
  for_each(myMap.begin(),myMap.end(),[&fSortWords](pair<string,int> mapPair){fSortWords.push_back(mapPair);});
  std::sort(fSortWords.begin(), fSortWords.end(), [] (pair<string, int> x, pair<string, int> y){return (x.second > y.second);});
  for_each(fSortWords.begin(), fSortWords.end(), printF);
}
else if(sort == "-o")
{
  printO(cleanedWords, n);
}



return 0;
}

string removeJunk(string& word)
{
  int start{};
  int end{};
  string junkStart{"\"'("};
  string junkEnd{"!?;,:.\"')"};
  string s{"s"};
  string apost{"'"};

  // Remove all the junk before the potential word
  start = word.find_first_of(junkStart);
  end = word.find_first_not_of(junkStart);
  if(start > -1 && end > start)
  {
    word.erase(start, (end - start));
  }

  // Remove all the junk after the potential word
  start = word.find_last_not_of(junkEnd);
  end = word.find_last_of(junkEnd);
  if(end + 1 == word.size())
  {
    word.erase(start + 1, (end - start));
  }

  // Remove possession/genitive
  start = word.find_last_of(apost);
  end = word.find_last_of(s);
  if(start + 1 == end && end + 1 == word.size())
  {
    word.erase(start, 2);
  }

  return word;
}

string validation(string& potentialWord)
{
  char dash{'-'};
  string doubleDash{"--"};
  if(potentialWord.size() < 3 || potentialWord.front() == dash || potentialWord.back() == dash 
    || potentialWord.find(doubleDash) != string::npos || isAlpha(potentialWord) == false )
  {
    return " ";
  }
  else
  {
    return potentialWord;
  }
}

string lowerCase(string& validWord)
{
  transform(validWord.begin(), validWord.end(), validWord.begin(),
                   [](unsigned char c) -> unsigned char { return tolower(c); });
  return validWord;
}

bool isAlpha(string& word)   
{
  bool check{};
  for_each(word.begin(),word.end(),[&check](char letter)
                                  {
                                    if(isalpha(letter) || letter == ' '|| letter == '-')
                                    {
                                      //Do Nothing
                                    }
                                    else
                                    {
                                      check = true;
                                    }  
                                  });
 
  if (!check)
  {
    return true;
  }
  return false;
}

void printA(pair<string, int> mapPair)
{
  cout << mapPair.first << setfill(' ') << setw(20 - mapPair.first.length()) << right << mapPair.second << endl;
}

void printF(pair<string, int> mapPair)
{
  cout << setfill(' ') << setw(15) << mapPair.first << setfill(' ') << setw(20) << right << mapPair.second << endl;
}

void printO(vector<string>& cleanedWords, int& n)
{
  int lines{};
  string words{};
  for_each(cleanedWords.begin(),cleanedWords.end(),[&lines,&n](string words)
                                                  {
                                                    if ((words.size() + lines) < n)
                                                      {
                                                        cout << words << " ";
                                                        lines += + words.size() + 1;
                                                      }
                                                    else
                                                      {
                                                        cout << endl << words << " ";
                                                        lines = words.size() + 1;
                                                      }
                                                  });

  cout << endl;
}
