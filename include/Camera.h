#ifndef _Cam_
#define _Cam_

#include <string>
#include <iostream>
#include <fstream>

struct Camera {
	std::string username;
	std::string password;
	std::string url;

	bool fileBoo;
	std::string fString;
	std::ofstream& file;

	bool errorBoo;
	std::string eString;
};

#endif // !_Cam_

