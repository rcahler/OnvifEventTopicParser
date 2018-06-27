#ifndef _ValCred_
#define _ValCred_

#include <string>

class ValidateCredentials {
public:
	ValidateCredentials(int argc, char* argv[]);
public:
	std::string username;
	std::string password;
	std::string url;

	bool fileBoo;
	std::string fString;

	bool errorBoo;
	std::string eString;
};
#endif