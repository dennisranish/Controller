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
	rawCode = (char*)code;

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

void Compiler::compileCode()
{
	TreeStructreReturn a = create(NULL, tst_scope, false, false, true, true, false, 7, false, true);

	//TODO: resolveNames (namespaces)
}

#include <iostream>

Compiler::TreeStructreReturn Compiler::create(TreeStructure *parent, TreeStructureTypes tsType, bool hasParent, bool singleLine, bool allowInstances, bool allowRunningCode, bool writeCodeToRoot, unsigned int startingSize, bool takeParameters, bool doLoop, unsigned int lutId = UINT32_MAX)
{
	TreeStructure* thisTS = new TreeStructure{ tsType, startingSize, hasParent, parent };
	if (writeCodeToRoot) thisTS->codeRoot = parent->codeRoot;
	else thisTS->codeRoot = thisTS;
	allTreeStructures.push_back(thisTS);

	while (tokenIndex < tokenStart.size() && doLoop)
	{
		if (checkForToken(tokenIndex, "{"))
		{
			if (!allowRunningCode) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			TreeStructreReturn newScope = create(thisTS, tst_scope, true, false, true, true, false, 7, false, true);
			if (newScope.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_newScope, newScope.ts->size, 0, 0, 0, newScope.ts->getCodeStart(&codePointerLut) });
		}
		else if (checkForToken(tokenIndex, "}"))
		{
			if (singleLine) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_endScope, 1 });
			return { thisTS, at_normal };
		}
		else if (checkForToken(tokenIndex, ";"))
		{
			tokenIndex += 1;
			if (singleLine)
			{
				thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_endScope, 1 });
				return { thisTS, at_normal };
			}
		}
		else if (checkForToken(tokenIndex, "object"))
		{
			tokenIndex += 1;
			unsigned int objectId = lutGetId(tokenIndex);
			thisTS->objectId.push_back(objectId);
			tokenIndex += 1;
			if (!checkForToken(tokenIndex, "{")) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			TreeStructreReturn newObject = create(thisTS, tst_objectType, true, false, true, false, false, 0, false, true, objectId);
			if (newObject.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->object.push_back(newObject.ts);
		}
		else if (checkForToken(tokenIndex, "nativeObject"))
		{
			tokenIndex += 1;
			unsigned int objectId = lutGetId(tokenIndex);
			thisTS->objectId.push_back(objectId);
			tokenIndex += 1;
			char *token = getToken(tokenIndex), *end;
			unsigned int objectSize = strtol(token, &end, 10);
			if (end - token != tokenEnd[tokenIndex] - tokenStart[tokenIndex] + 1) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			if (!checkForToken(tokenIndex, "{")) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			TreeStructreReturn newObject = create(thisTS, tst_objectType, true, false, true, false, false, objectSize, false, true, objectId);
			if (newObject.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->object.push_back(newObject.ts);
		}
		else if (checkForToken(tokenIndex, "nativeFunction"))
		{
			tokenIndex += 1;
			if (!isTokenObjectType(tokenIndex, thisTS)) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			char* fullName = getTokenFullName(tokenIndex, thisTS);
			bool foundNativeFunction = false;
			unsigned int nativeFunctionIndex;
			for (unsigned int i = 0; i < nativeFunctionCount; i++)
			{
				if (strcmp(fullName, nativeFunctionName[i]))
				{
					foundNativeFunction = true;
					nativeFunctionIndex = i;
					break;
				}
			}
			free(fullName);
			if(!foundNativeFunction) return { thisTS, at_unexpectedToken };
			unsigned int functionId = lutGetId(tokenIndex);
			thisTS->functionId.push_back(functionId);
			tokenIndex += 1;
			TreeStructreReturn newFunction = create(thisTS, tst_scope, true, false, true, true, false, 7, true, false, functionId);
			if (newFunction.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->function.push_back(newFunction.ts);
			newFunction.ts->addToCode({ Interpreter::Thread::cmd_callNative, nativeFunctionIndex });
			if (!checkForToken(tokenIndex, ";")) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
		}
		else if (checkForToken(tokenIndex, "name"))
		{
			tokenIndex += 1;
			unsigned int nameId = lutGetId(tokenIndex);
			thisTS->nameId.push_back(nameId);
			tokenIndex += 1;
			if (!checkForToken(tokenIndex, "{")) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			TreeStructreReturn newName = create(parent, tst_name, true, false, true, true, true, 0, false, true, nameId);
			if (newName.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->name.push_back(newName.ts);
		}
		else if (checkForToken(tokenIndex, "if"));
		else if (checkForToken(tokenIndex, "for"));
		else if (checkForToken(tokenIndex, "while"));
		else if (checkForToken(tokenIndex, "break"));
		else if (checkForToken(tokenIndex, "continue"));
		else if (checkForToken(tokenIndex, "return"));
		else if (isTokenObjectType(tokenIndex, thisTS))
		{
			TreeStructure *object = lastToken;
		}
		else { std::cout << "Compiler is not done. Encountered code that is either wrong or cannot yet be properly compiled." << std::endl; return { thisTS, at_unexpectedToken }; }
	}
}

bool Compiler::checkForToken(unsigned int index, const char* token)
{
	for (unsigned int i = 0; token[i] != '\0' && tokenStart[index] + i <= tokenEnd[index]; i++)
	{
		if (rawCode[tokenStart[index] + i] != token[i]) return false;
		if (tokenStart[index] + i == tokenEnd[index] && token[i + 1] == '\0') return true;
	}
	return false;
}

bool Compiler::isTokenObjectType(unsigned int index, TreeStructure *thisTS)
{
	bool found = false;

	while (!found)
	{
		for (unsigned int objectIndex = 0; objectIndex < thisTS->objectId.size(); objectIndex++) if (checkForToken(index, lut[thisTS->objectId[objectIndex]]))
		{
			lastToken = thisTS->object[objectIndex];
			return true;
		}
		if(!found) for (unsigned int nameIndex = 0; nameIndex < thisTS->nameId.size(); nameIndex++) if (checkForToken(index, lut[thisTS->nameId[nameIndex]])) found = true;
		if (!found) for (unsigned int instanceIndex = 0; instanceIndex < thisTS->instanceId.size(); instanceIndex++) if (checkForToken(index, lut[thisTS->instanceId[instanceIndex]])) return false;
		if (!found) for (unsigned int functionIndex = 0; functionIndex < thisTS->functionId.size(); functionIndex++) if (checkForToken(index, lut[thisTS->functionId[functionIndex]])) return false;

		if (!thisTS->hasParent) break;
		if (!found) thisTS = thisTS->parent;
	}

	unsigned int offset = 0;

	while(found)
	{
		found = false;

		if (!checkForToken(index + offset + 1, ".")) return false;
		offset += 2;

		for (unsigned int objectIndex = 0; objectIndex < thisTS->objectId.size(); objectIndex++) if (checkForToken(index + offset, lut[thisTS->objectId[objectIndex]]))
		{
			lastToken = thisTS->object[objectIndex];
			return true;
		}
		if (!found) for (unsigned int nameIndex = 0; nameIndex < thisTS->nameId.size(); nameIndex++) if (checkForToken(index + offset, lut[thisTS->nameId[nameIndex]])) found = true;
		if (!found) for (unsigned int instanceIndex = 0; instanceIndex < thisTS->instanceId.size(); instanceIndex++) if (checkForToken(index + offset, lut[thisTS->instanceId[instanceIndex]])) return false;
		if (!found) for (unsigned int functionIndex = 0; functionIndex < thisTS->functionId.size(); functionIndex++) if (checkForToken(index + offset, lut[thisTS->functionId[functionIndex]])) return false;
	}

	return false;
}

char* Compiler::getTokenFullName(unsigned int index, TreeStructure *thisTS)
{
	char* fullName;
	unsigned int fullNameSize = tokenEnd[index] - tokenStart[index] + 1;
	std::vector<unsigned int> lutIdNames;
	std::vector<unsigned int> lutIdSize;

	while (thisTS->type == tst_name || thisTS->type == tst_objectType)
	{
		lutIdNames.push_back(thisTS->lutId);
		lutIdSize.push_back(strlen(lut[thisTS->lutId]));
		fullNameSize += lutIdSize[lutIdNames.size() - 1] + 1;

		if (thisTS->type == tst_objectType) break;
		if (!thisTS->hasParent) break;
		thisTS = thisTS->parent;
	}

	fullName = (char*)malloc(fullNameSize + 1);
	
	unsigned int fullNameIndex = 0;

	for (unsigned int i = 0; i < lutIdNames.size(); i++)
	{
		for (unsigned int a = 0; a < lutIdSize[i]; a++) fullName[fullNameIndex + a] = lut[lutIdNames[i]][a];
		fullNameIndex += lutIdSize[i] + 1;
		fullName[fullNameIndex - 1] = '.';
	}

	for (unsigned int i = 0; i <= tokenEnd[index] - tokenStart[index]; i++) fullName[fullNameIndex + i] = rawCode[tokenStart[index] + i];
	fullName[fullNameSize - 1] = '\0';

	return fullName;
}

void Compiler::addNativeFunction(const char* name, void(*function)(uintptr_t parentScope, uintptr_t previousScope, unsigned int parameterCount, uintptr_t thisPointer))
{
	if (nativeFunctionCount == 0)
	{
		nativeFunction = (void(**)(uintptr_t parentScope, uintptr_t previousScope, unsigned int parameterCount, uintptr_t thisPointer))malloc(4);
		nativeFunctionName = (char**)malloc(4);
	}
	else
	{
		nativeFunction = (void(**)(uintptr_t parentScope, uintptr_t previousScope, unsigned int parameterCount, uintptr_t thisPointer))realloc(nativeFunction, nativeFunctionCount * 4 + 4);
		nativeFunctionName = (char**)realloc(nativeFunctionName, nativeFunctionCount * 4 + 4);
	}

	nativeFunctionCount++;
	nativeFunction[nativeFunctionCount - 1] = function;
	nativeFunctionName[nativeFunctionCount - 1] = (char*)name;
}

unsigned int Compiler::lutGetId(unsigned int index)
{
	for (unsigned int lutIndex = 0; lutIndex < lut.size(); lutIndex++)
	{
		for (unsigned int i = 0; lut[i] != '\0' && tokenStart[index] + i <= tokenEnd[index]; i++)
		{
			if (rawCode[tokenStart[index] + i] != lut[lutIndex][i]) break;
			if (tokenStart[index] + i == tokenEnd[index] && lut[lutIndex][i + 1] == '\0') return lutIndex;
		}
	}

	char *lutToken = (char*)malloc((tokenEnd[index] - tokenStart[index]) + 2);
	memcpy(lutToken, &rawCode[tokenStart[index]], (tokenEnd[index] - tokenStart[index]) + 1);
	lutToken[(tokenEnd[index] - tokenStart[index]) + 1] = '\0';

	lut.push_back(lutToken);
	return lut.size() - 1;
}