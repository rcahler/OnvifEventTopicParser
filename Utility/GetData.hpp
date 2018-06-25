#pragma once
#include <string>
#include "parson.h"

void GetData(std::string user, std::string pass, std::string url);
void ToJsonTopicTwoElements(std::string, std::vector<std::pair<std::string, std::string>>);
void ToJsonTopicMoreElements(std::string, std::vector<std::pair<std::string, std::string>>);
void ToJsonTopicLessElements(std::string, std::vector<std::pair<std::string, std::string>>);

bool IsMotion(std::string);
bool IsInTrig(std::string);
