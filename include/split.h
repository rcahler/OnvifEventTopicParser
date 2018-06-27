#pragma once

#ifndef _SPLIT_
#define _SPLIT_

#include <string>
#include <vector>

//Splits a string into vectors at every occurence of a specified delimiter 
std::vector<std::string> split(const std::string &s, char delim);

#endif // !_SPLIT_
