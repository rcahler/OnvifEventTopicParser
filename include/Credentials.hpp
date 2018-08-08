#pragma once

#include <string>

class Credentials {
public:
	Credentials(int argc, char* argv[]);
	int ValidateCreds();
private:
	char* ip;
	bool help = false;
	char* port;
public:
	char* username;
	char* password;
	std::string url;
	char* filepath;
	bool verbose = false;
};

void printCredErrors(int);
void printHelp();