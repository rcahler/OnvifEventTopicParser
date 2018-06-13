#ifndef _Cam_
#define _Cam_

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

struct CameraError {
	bool boo;
	string eString;
};

struct FilePath {
	bool boo;
	string fString;
	ofstream file;
};

struct Camera {
	string username;
	string password;
	string url;
	FilePath path;
	CameraError error;
};

#endif // !_Cam_

