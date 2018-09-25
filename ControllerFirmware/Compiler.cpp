#include "Compiler.h"
bool Compiler::cmpstr(char* a, char* b)
{
	for(int i = 0;a[i] != '\0' && b[i] != '\0';i++) if(a[i] != b[i]) return false;
	return true;
}

void Compiler::parseCode(char* code)
{
	unsigned int previousCharacterType = 0;
	unsigned int index = 0;
	unsigned int codeLength = strlen(code);
	for(; index <= codeLength; index++)
	{
		if(tokenStart.size() >= tokenStart.capacity())
		{
			tokenStart.reserve(tokenStart.size() + 100);
			tokenEnd.reserve(tokenStart.size() + 100);
			isTokenString.reserve(tokenStart.size() + 100);
		}
		
		//Handle empty spaces
		bool foundEmpty = false;
		for(int i = 0; i < emptyRule.size(); i++)
		{
			if(code[index] == emptyRule[i])
			{
				if(tokenEnd.size() < tokenStart.size()) tokenEnd.push_back(index - 1);
				foundEmpty = true;
				previousCharacterType = standardRule.size() + 1;
				break;
			}
		}
		if(foundEmpty == true) continue;
		
		//Handle comments
		bool foundComment = false;
		for(int i = 0; i < commentRuleStart.size(); i++)
		{
			if(cmpstr(&code[index], commentRuleStart[i]))
			{
				if(tokenEnd.size() < tokenStart.size()) tokenEnd.push_back(index - 1);
				index += strlen(commentRuleStart[i]);
				while(!cmpstr(&code[index], commentRuleEnd[i]) && index <= codeLength) index++;
				index += strlen(commentRuleEnd[i]) - 1; //- 1 because continue will be call which will cause index++
				foundComment = true;
				previousCharacterType = standardRule.size() + 1;
				break;
			}
		}
		if(foundComment == true) continue;
		
		//Handle strings
		bool foundString = false;
		for(int i = 0; i < stringRuleStart.size(); i++)
		{
			if(cmpstr(&code[index], stringRuleStart[i]))
			{
				if(tokenEnd.size() < tokenStart.size()) tokenEnd.push_back(index - 1);
				index += strlen(stringRuleStart[i]);
				tokenStart.push_back(index);
				isTokenString.push_back(true);
				while(!cmpstr(&code[index], stringRuleEnd[i]) && index <= codeLength)
				{
					if(cmpstr(&code[index], stringRuleException[i])) index += strlen(stringRuleException[i]);
					else index++;
				}
				tokenEnd.push_back(index - 1);
				index += strlen(stringRuleEnd[i]) - 1; //- 1 because continue will be call which will cause index++
				foundString = true;
				previousCharacterType = standardRule.size() + 1;
				break;
			}
		}
		if(foundString == true) continue;

		unsigned int characterType = 0;
		bool foundCharacterType = false;
		for(int i = 0; i < standardRule.size() && !foundCharacterType; i++)
		{
			unsigned int ruleLength = strlen(standardRule[i]);

			for(int c = 0; c <= ruleLength; c++)
			{
				if(standardRuleRange[i])
				{
					if(code[index] >= standardRule[i][c] && code[index] <= standardRule[i][c + 1])
					{
						characterType = i;
						foundCharacterType = true;
						break;
					}

					c++;
				}
				else if(code[index] == standardRule[i][c])
				{
					characterType = i;
					foundCharacterType = true;
					break;
				}
			}
		}
		
		if(!foundCharacterType)
		{
			if(!includeRest)
			{
				if(tokenEnd.size() < tokenStart.size()) tokenEnd.push_back(index - 1);
			}

			if(includeRest && (standardRule.size() != previousCharacterType || (tokenEnd.size() - tokenStart.size() >= restTokenLength && restTokenLength != -1)))
			{
				if(tokenEnd.size() < tokenStart.size()) tokenEnd.push_back(index - 1);
				tokenStart.push_back(index);
				isTokenString.push_back(false);
				previousCharacterType = standardRule.size();
			}
		}
		else if(characterType != previousCharacterType || (tokenEnd.size() - tokenStart.size() >= standardRuleTokenLength[characterType] && standardRuleTokenLength[characterType] != -1))
		{
			if(tokenEnd.size() < tokenStart.size()) tokenEnd.push_back(index - 1);
			tokenStart.push_back(index);
			isTokenString.push_back(false);
			previousCharacterType = characterType;
		}
	}

	if(tokenEnd.size() < tokenStart.size()) tokenEnd.push_back(codeLength);

	tokenStart.shrink_to_fit();
	tokenEnd.shrink_to_fit();
	isTokenString.shrink_to_fit();
}
