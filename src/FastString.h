#ifndef FastString_h
#define FastString_h

#include <Arduino.h>

class FastString
{
	public:
		//static char* add(std::initializer_list<char*> stringList);
		static char* add(std::vector<char*> stringList);
};

#endif