#ifndef Utils_h
#define Utils_h

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

class Utils
{
public:
	static float degreesToRadians(int deg) { return deg * (3.14159265358f / 180.0f); }
	static std::string ReadFile(const char *path);
};




#endif // !Utils_h



