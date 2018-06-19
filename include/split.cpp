#include "split.h"
#include <string>
#include <vector>
#include <sstream>

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> vector;
	std::stringstream stream(s);
	std::string between;

	while (getline(stream, between, delim)) {
		vector.push_back(between);
	}

	return vector;
}