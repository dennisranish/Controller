#include "FastString.h"

/*char* FastString::add(std::initializer_list<char*> stringList)
{
	int size = 0;
	for(int i = 0; i < stringList.size(); i++) size += strlen(stringList.begin()[i]);
	char * newString = (char *)malloc(size + 1);
	int charPosition = 0;
	for(int i = 0; i < stringList.size(); i++)
	{
		for(int c = 0; stringList.begin()[i][c] != '\0'; c++)
		{
			newString[charPosition] = stringList.begin()[i][c];
			charPosition++;
		}
	}
	newString[charPosition] = '\0';
	return newString;
}*/

char* FastString::add(std::vector<char*> stringList)
{
	int size = 0;
	for(int i = 0; i < stringList.size(); i++) size += strlen(stringList[i]);
	char * newString = (char*)malloc(size + 1);
	int charPosition = 0;
	for(int i = 0; i < stringList.size(); i++)
	{
		for(int c = 0; stringList[i][c] != '\0'; c++)
		{
			newString[charPosition] = stringList[i][c];
			charPosition++;
		}
	}
	newString[charPosition] = '\0';
	return newString;
}