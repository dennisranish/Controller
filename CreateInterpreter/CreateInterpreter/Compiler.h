#ifndef Compiler_h
#define Compiler_h

//#include "Arduino.h" //Add for arduino
#include "vector"//Remove for arduino
#include <stdarg.h>

class Compiler
{
	public:
		void addStandardRule(std::initializer_list<const char*> rule, char maxTokenLength = 0);
		void addWrapRule(const char* start, const char* end, bool exclude, std::initializer_list<const char*> exceptions = { "" }, char maxTokenLength = 0);

		bool compareStringTillEnd(const char* stringA, const char* stringB);
		unsigned int findStringInVector(const char* stringA, std::vector<char*> stringVector);
		bool checkCharInRange(const char charA, const char* stringRange);
		unsigned int findStringInRule(const char* stringA);
		void parseCode(const char* code);

		unsigned int getSize(){return tokenStart.size();}
		char* getToken(unsigned int index, char* programCode)
		{	
			char* tokenValue = (char*)malloc((tokenEnd[index] - tokenStart[index]) + 2);
		
			for(unsigned int i = 0; i <= (tokenEnd[index] - tokenStart[index]); i++) tokenValue[i] = programCode[tokenStart[index] + i];
			tokenValue[(tokenEnd[index] - tokenStart[index]) + 1] = '\0';
		
			return tokenValue;
		}
		
	private:
		std::vector<std::vector<char*>> standardRule;
		std::vector<char> standardRuleMaxTokenLength;

		std::vector<char*> wrapRuleStart;
		std::vector<char*> wrapRuleEnd;
		std::vector<bool> wrapRuleExclude;
		std::vector<std::vector<char*>> wrapRuleExceptions;
		std::vector<char> wrapRuleMaxTokenLength;

		std::vector<unsigned int> tokenStart;
		std::vector<unsigned int> tokenEnd;
};

#endif
