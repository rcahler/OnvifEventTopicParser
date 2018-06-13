#include "split.h"
#include <string>
#include <vector>
#include <sstream>

using namespace std;

vector<string> split(const string &s, char delim) {
	vector<string> vector;
	stringstream stream(s);
	string between;

	while (getline(stream, between, delim)) {
		vector.push_back(between);
	}

	return vector;
}