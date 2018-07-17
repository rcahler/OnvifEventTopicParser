#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "split.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

int main(int argc, char* argv[]) {

	if (argc != 2) {
		std::cerr << "Wrong number of arguements";
		return 0;
	}

	//Make sure the arguement is a valid textfile
	char* filename = argv[1];

	if (strlen(filename) < 5) {
		std::cerr << "Enter a valid .txt textFile";
		return 0;
	}
	
	char filetype[5];

	int j = 0;
	for (size_t i = strlen(filename) - 4; i < strlen(filename); ++i) {
		filetype[j] = filename[i];
		++j;
	}

	filetype[4] = '\0'; //Null terminator
	if (!strcmp(filetype, ".txt") == 0) {
		std::cerr << "Not a .txt textFile";
		return 0;
	}

	
	std::ifstream textFile(filename);
	std::string line;

	char* outputFilename = NULL;
	int lineNumber = 1;

	while (std::getline(textFile, line))
	{

		std::stringstream ss(line);
		std::string line = ss.str();
		std::vector<std::string> split_line = split(line, ' ');

		/*
		for (size_t i = 0; i < split_line.size(); ++i) {
			std::cout << split_line[i] << " ";
		}
		*/

		if (split_line.size() == 1) { //Should only be the initial line
			if (outputFilename != NULL) {
				std::cerr << "Issue on line #" << lineNumber << ", too few arguements. This line will be skipped";
			}
			else {
				int len = strlen(split_line[0].c_str()) + 1;
				outputFilename = new char[len];
				strcpy_s(outputFilename, len, split_line[0].c_str());
				//std::cout << outputFilename << std::endl;
			}
		}
		else if (split_line.size() == 3) { //Should only be the initial line
			char* username = new char[strlen(split_line[0].c_str()) + 1];
			strcpy_s(username, strlen(split_line[0].c_str()) + 1, split_line[0].c_str());

			char* password = new char[strlen(split_line[1].c_str())];
			strcpy_s(password, strlen(split_line[1].c_str()) + 1, split_line[1].c_str());

			char* ip = new char[strlen(split_line[2].c_str())];
			strcpy_s(ip, strlen(split_line[2].c_str()) + 1, split_line[2].c_str());

			std::string cl = "Utility.exe -u " + split_line[0] + " -p " + split_line[1] + " -ip " + split_line[2] + " -f " + outputFilename;

			system(cl.c_str());
			
		}
		else if (split_line.size() <= 2) {
			char* username = new char[strlen(split_line[0].c_str()) + 1];
			strcpy_s(username, strlen(split_line[0].c_str()) + 1, split_line[0].c_str());

			char* ip = new char[strlen(split_line[1].c_str())];
			strcpy_s(ip, strlen(split_line[1].c_str()) + 1, split_line[1].c_str());

			std::string cl = "Utility.exe -u " + split_line[0] + " -ip " + split_line[1] + " -f " + outputFilename;

			system(cl.c_str());
		}

		lineNumber += 1;

	}

	return 0;
}