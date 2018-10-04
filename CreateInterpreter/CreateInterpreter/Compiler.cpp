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

unsigned int Compiler::findStringInRule(const char* stringA)
{
	size_t aSize = standardRule.size();
	for (unsigned int a = 0; a < aSize; a++)
	{
		size_t bSize = standardRule[a].size();
		for (unsigned int b = 0; b < bSize; b++)
		{
			if (bSize == 1 && checkCharInRange(stringA[b], standardRule[a][b])) return a;
			else if (bSize == 1) break;

			if (b == bSize - 1) return a;
			if (!checkCharInRange(stringA[b], standardRule[a][b])) break;
		}
	}

	return aSize;
}

void Compiler::parseCode(const char* code)
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

			continue;
		}

		//standardRule
		unsigned int standardRuleIndex = findStringInRule(&code[index]);
		if (standardRuleIndex != standardRule.size())
		{
			tokenStart.push_back(index);

			unsigned int characterCount = 0;

			while (checkCharInRange(code[index], standardRule[standardRuleIndex].back()) && index <= codeLength)
			{
				if (findStringInVector(&code[index], wrapRuleStart) != wrapRuleStart.size()) break;

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