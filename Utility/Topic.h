#pragma once
#include <string>
#include <vector>

class Topic {
public:
	std::string name;
	std::vector<std::pair<std::string, std::string>> elements;
};
