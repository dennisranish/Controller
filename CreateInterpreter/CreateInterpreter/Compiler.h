#ifndef Compiler_h
#define Compiler_h

//#include "Arduino.h" //Add for arduino
#include "vector"//Remove for arduino

class Compiler
{
	public:
		//void clearRules(){standardRule.clear();standardRuleTokenLength.clear();emptyRule.clear();commentRuleStart.clear();commentRuleEnd.clear();stringRuleStart.clear();stringRuleEnd.clear();stringRuleException.clear();stringRuleTokenLength.clear();includeRest = false;restTokenLength = -1;}
		//void addRule(char* rule, int maxTokenLength, bool range = true){if(rule[0] == '\0'){includeRest = true;restTokenLength = maxTokenLength;return;}standardRule.push_back(rule);standardRuleTokenLength.push_back(maxTokenLength);standardRuleRange.push_back(range);}
		//void addRuleEmpty(char* rule){emptyRule.insert(emptyRule.end(), rule, rule + strlen(rule));}
		//void addRuleComment(char* ruleStart, char* ruleEnd){commentRuleStart.push_back(ruleStart);commentRuleEnd.push_back(ruleEnd);}
		//void addRuleString(char* ruleStart, char* ruleEnd, char* ruleException, int maxTokenLength){stringRuleStart.push_back(ruleStart);stringRuleEnd.push_back(ruleEnd);stringRuleException.push_back(ruleException);stringRuleTokenLength.push_back(maxTokenLength);}

		void addStandardRule(std::initializer_list<const char*> rule, char maxTokenLength = 0);
		void addWrapRule(const char* start, const char* end, bool exclude, std::initializer_list<const char*> exceptions = { "" }, char maxTokenLength = 0);

		bool compareStringTillEnd(const char* stringA, const char* stringB);
		unsigned int findStringInVector(const char* stringA, std::vector<char*> stringVector);
		bool checkCharInRange(const char charA, const char* stringRange);
		unsigned int findStringInRule(const char* stringA, std::vector<std::vector<char*>> stringVector);
		void parseCode(const char* code);

		//bool cmpstr(char* a, char* b);
		//void parseCode1(char* code);
		//void parseCode2(char* code);

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

		/*std::vector<char*> standardRule;
		std::vector<int> standardRuleTokenLength;
		std::vector<bool> standardRuleRange;
		std::vector<char> emptyRule;
		std::vector<char*> commentRuleStart;
		std::vector<char*> commentRuleEnd;
		std::vector<char*> stringRuleStart;
		std::vector<char*> stringRuleEnd;
		std::vector<char*> stringRuleException;
		std::vector<int> stringRuleTokenLength;
		bool includeRest = false;
		int restTokenLength = -1;*/

		std::vector<unsigned int> tokenStart;
		std::vector<unsigned int> tokenEnd;
		//std::vector<bool> isTokenString;
};

#endif
