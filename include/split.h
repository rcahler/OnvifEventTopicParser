#pragma once

#ifndef _SPLIT_
#define _SPLIT_

#include <string>
#include <vector>

using namespace std;

//Splits a string into vectors at every occurence of a specified delimiter 
vector<string> split(const string &s, char delim);

#endif // !_SPLIT_
