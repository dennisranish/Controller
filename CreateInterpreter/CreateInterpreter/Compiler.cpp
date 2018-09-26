#include "Compiler.h"

void Compiler::addStandardRule(std::initializer_list<const char*> rule, char maxTokenLength)
{
	std::vector<char*> newRule;
	size_t ruleSize = rule.size();
	const char* const* ruleBegin = rule.begin();
	for (unsigned int i = 0; i < ruleSize; i++) newRule.push_back((char*)ruleBegin[i]);
	standardRule.push_back(newRule);
	standardRuleMaxTokenLength.push_back(maxTokenLength);
}

void Compiler::addWrapRule(const char* start, const char* end, bool exclude, std::initializer_list<const char*> exception, char maxTokenLength)
{
	wrapRuleStart.push_back((char*)start);
	wrapRuleEnd.push_back((char*)end);
	wrapRuleExclude.push_back(exclude);

	std::vector<char*> newException;
	size_t exceptionSize = exception.size();
	const char* const* exceptionBegin = exception.begin();
	for (unsigned int i = 0; i < exceptionSize; i++) newException.push_back((char*)exceptionBegin[i]);
	wrapRuleExceptions.push_back(newException);

	wrapRuleMaxTokenLength.push_back(maxTokenLength);
}

bool Compiler::compareStringTillEnd(const char* stringA, const char* stringB)
{
	for (unsigned int i = 0; stringA[i] != '\0' && stringB[i] != '\0'; i++)
	{
		if (stringA[i] != stringB[i]) return false;
	}
	return true;
}

unsigned int Compiler::findStringInVector(const char* stringA, std::vector<char*> stringVector)
{
	size_t stringVectorSize = stringVector.size();
	for (unsigned int i = 0; i < stringVectorSize; i++)
	{
		if (compareStringTillEnd(stringA, stringVector[i])) return i;
	}

	return stringVectorSize;
}

bool Compiler::checkCharInRange(const char charA, const char* stringRange)
{
	unsigned int i = 0;
	bool inRange = false;
	while (stringRange[i] != '\0')
	{
		if (charA >= stringRange[i] && charA <= stringRange[i + 1]) inRange = true;
		i += 2;
	}
	return inRange;
}

unsigned int Compiler::findStringInRule(const char* stringA, std::vector<std::vector<char*>> stringRule)
{
	size_t aSize = stringRule.size();
	for (unsigned int a = 0; a < aSize; a++)
	{
		size_t bSize = stringRule[a].size();
		for (unsigned int b = 0; b < bSize; b++)
		{
			if (bSize == 1 && checkCharInRange(stringA[b], stringRule[a][b])) return a;
			else if (bSize == 1) break;

			if (b == bSize - 1) return a;
			if (!checkCharInRange(stringA[b], stringRule[a][b])) break;
		}
	}

	return aSize;
}

void Compiler::parseCode1(const char* code)
{
	unsigned int index = 0;
	unsigned int codeLength = strlen(code);
	for (; index <= codeLength; index++)
	{
		if (tokenStart.size() >= tokenStart.capacity())
		{
			tokenStart.reserve(tokenStart.size() + 100);
			tokenEnd.reserve(tokenStart.size() + 100);
		}

		//wrapRule
		unsigned int warpRuleIndex = findStringInVector(&code[index], wrapRuleStart);
		if (warpRuleIndex != wrapRuleStart.size())
		{
			if (!wrapRuleExclude[warpRuleIndex]) tokenStart.push_back(index);

			index += strlen(wrapRuleStart[warpRuleIndex]);

			unsigned int characterCount = 0;

			while (!compareStringTillEnd(wrapRuleEnd[warpRuleIndex], &code[index]) && index <= codeLength)
			{
				if (wrapRuleMaxTokenLength[warpRuleIndex] != 0 && characterCount >= wrapRuleMaxTokenLength[warpRuleIndex]) break;

				index++;
				characterCount++;

				unsigned int wrapRuleExceptionIndex = findStringInVector(&code[index], wrapRuleExceptions[warpRuleIndex]);
				if (wrapRuleExceptionIndex != wrapRuleExceptions[warpRuleIndex].size())
				{
					index += strlen(wrapRuleExceptions[warpRuleIndex][wrapRuleExceptionIndex]);
				}
			}

			index += strlen(wrapRuleEnd[warpRuleIndex]) - 1;

			if (!wrapRuleExclude[warpRuleIndex]) tokenEnd.push_back(index);
		}

		//standardRule
		unsigned int standardRuleIndex = findStringInRule(&code[index], standardRule);
		if (standardRuleIndex != standardRule.size())
		{
			tokenStart.push_back(index);

			unsigned int characterCount = 0;

			while (checkCharInRange(code[index], standardRule[standardRuleIndex].back()) && index <= codeLength)
			{
				if (standardRuleMaxTokenLength[standardRuleIndex] != 0 && characterCount >= standardRuleMaxTokenLength[standardRuleIndex]) break;

				index++;
				characterCount++;
			}

			index--;

			tokenEnd.push_back(index);
		}
	}

	tokenStart.shrink_to_fit();
	tokenEnd.shrink_to_fit();
}

void Compiler::parseCode(const char* code)
{
	unsigned int ruleCount = rule.size();
	unsigned int codeLength = strlen(code);
	for (unsigned int index = 0; index <= codeLength; index++)
	{
		if (tokenStart.size() >= tokenStart.capacity())
		{
			tokenStart.reserve(tokenStart.size() + 100);
			tokenEnd.reserve(tokenStart.size() + 100);
		}

		unsigned int startIndex = index;

		for (unsigned int ruleIndex = 0; ruleIndex < ruleCount; ruleIndex++)
		{
			index = startIndex;
			bool found = false;
			bool inLoop = false;
			bool untilLoop = false;
			bool lookForLoopEnd = false;
			unsigned int loopIndex = 0;
			bool complete = false;
			bool addToken = false;

			for(unsigned int i = 0; index <= codeLength; i++)
			{
				if (lookForLoopEnd && rule[ruleIndex][i] != 3 && rule[ruleIndex][i] != 7) continue;
				else if (lookForLoopEnd && rule[ruleIndex][i] == 3)
				{
					lookForLoopEnd = false;
					continue;
				}

				if (rule[ruleIndex][i] == 0)//0: then
				{
					if (found == false && inLoop == false) break;
					else if (found == false && inLoop == true && untilLoop == true) i = loopIndex;
					else if (found == false && inLoop == true && untilLoop == false) lookForLoopEnd = true;
					found = false;
					index++;
				}
				else if (rule[ruleIndex][i] == 1)//1: loopWhile
				{
					inLoop = true;
					untilLoop = false;
					loopIndex = i;
				}
				else if (rule[ruleIndex][i] == 2)//2: loopUntil
				{
					inLoop = true;
					untilLoop = true;
					loopIndex = i;
				}
				else if (rule[ruleIndex][i] == 3)//3: loopEnd
				{
					if (found == false && untilLoop == true) i = loopIndex;
					else if (found == true && untilLoop == false) i = loopIndex;
					else inLoop = false;
					found = false;
					index++;
				}
				else if (rule[ruleIndex][i] == 4)//4: goBack
				{
					i++;
					index -= rule[ruleIndex][i + 1];
				}
				else if (rule[ruleIndex][i] == 5)//5: addToken
				{
					complete = true;
					addToken = true;
					break;
				}
				else if (rule[ruleIndex][i] == 6)//6: discardToken
				{
					complete = true;
					addToken = false;
					break;
				}
				else if (rule[ruleIndex][i] == 7)//7: endOfRule
				{
					break;
				}
				else if (rule[ruleIndex][i] == 8)//8: range
				{
					if (found == false)
					{
						if (code[index] >= rule[ruleIndex][i + 1] && code[index] <= rule[ruleIndex][i + 2]) found = true;
					}
					i += 2;
				}
				else if (rule[ruleIndex][i] == 9)//9: single
				{
					if (found == false)
					{
						if (code[index] == rule[ruleIndex][i + 1]) found = true;
					}
					i += 1;
				}
				else if (rule[ruleIndex][i] == 10)//10: !range
				{
					if (found == false)
					{
						if (code[index] < rule[ruleIndex][i + 1] || code[index] > rule[ruleIndex][i + 2]) found = true;
					}
					i += 2;
				}
				else if (rule[ruleIndex][i] == 11)//11: !single
				{
					if (found == false)
					{
						if (code[index] != rule[ruleIndex][i + 1]) found = true;
					}
					i += 1;
				}
			}

			if (complete)
			{
				if (addToken)
				{
					tokenStart.push_back(startIndex);
					tokenEnd.push_back(index);
				}

				break;
			}
		}
	}

	tokenStart.shrink_to_fit();
	tokenEnd.shrink_to_fit();
}



/*bool Compiler::cmpstr(char* a, char* b)
{
	for(int i = 0;a[i] != '\0' && b[i] != '\0';i++) if(a[i] != b[i]) return false;
	return true;
}

void Compiler::parseCode2(char* code)
{
	unsigned int previousCharacterType = 0;
	unsigned int index = 0;
	unsigned int codeLength = strlen(code);
	for (; index <= codeLength; index++)
	{
		if (tokenStart.size() >= tokenStart.capacity())
		{
			tokenStart.reserve(tokenStart.size() + 100);
			tokenEnd.reserve(tokenStart.size() + 100);
			isTokenString.reserve(tokenStart.size() + 100);
		}

		if (cmpstr(&code[index], (char*)"/*"))
		{
			*///while (!cmpstr(&code[index], (char*)"*/") && index <= codeLength) index++;
			/*index += 2;
		}
		else if (cmpstr(&code[index], (char*)"//"))
		{
			while (code[index] != '\n' && index <= codeLength) index++;
			index--;
		}
		else if (code[index] == (char)"\"")
		{
			tokenStart.push_back(index);
			while ((code[index] != (char)"\"" || cmpstr(&code[index], (char*)"\\\"")) && index <= codeLength) index++;
			index--;
			tokenEnd.push_back(index);
		}
		else if (code[index] == (char)"'")
		{
			tokenStart.push_back(index);
			while ((code[index] != (char)"'" || cmpstr(&code[index], (char*)"\\'")) && index <= codeLength) index++;
			index--;
			tokenEnd.push_back(index);
		}
		else if (code[index] == '{' || code[index] == '}' || code[index] == '[' || code[index] == ']' || code[index] == '(' || code[index] == ')' || code[index] == '.' || code[index] == ';')
		{
			tokenStart.push_back(index);
			tokenEnd.push_back(index);
		}
		else if ((code[index] >= 'a' && code[index] <= 'z') || (code[index] >= 'A' && code[index] <= 'Z') || (code[index] >= '0' && code[index] <= '9') || (code[index] == '_'))
		{
			tokenStart.push_back(index);
			while (((code[index] >= 'a' && code[index] <= 'z') || (code[index] >= 'A' && code[index] <= 'Z') || (code[index] >= '0' && code[index] <= '9') || (code[index] == '_')) && index <= codeLength) index++;
			index--;
			tokenEnd.push_back(index);
		}
		else if((code[index] >= '!' && code[index] <= '/') || (code[index] >= ':' && code[index] <= '@') || (code[index] == '\\') || (code[index] >= '^' && code[index] <= '`') || (code[index] == '|') || (code[index] == '~'))
		{
			tokenStart.push_back(index);
			while (((code[index] >= '!' && code[index] <= '/') || (code[index] >= ':' && code[index] <= '@') || (code[index] == '\\') || (code[index] >= '^' && code[index] <= '`') || (code[index] == '|') || (code[index] == '~')) && index <= codeLength) index++;
			index--;
			tokenEnd.push_back(index);
		}
	}

	tokenStart.shrink_to_fit();
	tokenEnd.shrink_to_fit();
	isTokenString.shrink_to_fit();
}

void Compiler::parseCode1(char* code)
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
}*/
