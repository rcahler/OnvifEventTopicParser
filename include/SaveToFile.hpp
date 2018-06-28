#pragma once
#include "Parson.h"
#include <string>

std::wstring ToWstring(std::string);
void SaveToFile(std::string, std::string, JSON_Value*);
