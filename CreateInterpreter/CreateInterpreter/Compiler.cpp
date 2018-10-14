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

			if (!wrapRuleExclude[warpRuleIndex])
			{
				tokenEnd.push_back(index);
				tokenType.push_back(standardRule.size() + warpRuleIndex);
			}

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
			tokenType.push_back(standardRuleIndex);
		}
	}

	tokenStart.shrink_to_fit();
	tokenEnd.shrink_to_fit();
}

void Compiler::compileCode()
{
	TreeStructreReturn a = create(NULL, tst_scope, false, false, true, true, false, Interpreter::Thread::ScopeMinSize, false, true);

	//TODO: resolveNames (namespaces)
}

Compiler::TreeStructreReturn Compiler::create(TreeStructure *parent, TreeStructureTypes tsType, bool hasParent, bool singleLine, bool allowInstances, bool allowRunningCode, bool writeCodeToRoot, unsigned int startingSize, bool takeParameters, bool doLoop, unsigned int lutId)
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
			TreeStructreReturn newScope = create(thisTS, tst_scope, true, false, true, true, false, Interpreter::Thread::ScopeMinSize, false, true);
			if (newScope.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_newScope, newScope.ts->size, 0, newScope.ts->codeRoot->getCodeStart(&codePointerLut) });
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
			if(tokenType[tokenIndex] != 0) return { thisTS, at_unexpectedToken };
			unsigned int objectId = lutGetId(tokenIndex);
			thisTS->objectId.push_back(objectId);
			tokenIndex += 1;
			if (!checkForToken(tokenIndex, "{")) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			TreeStructreReturn newObject = create(thisTS, tst_objectType, true, false, true, false, true, 0, false, true, objectId);
			if (newObject.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->object.push_back(newObject.ts);
		}
		else if (checkForToken(tokenIndex, "nativeObject"))
		{
			tokenIndex += 1;
			if (tokenType[tokenIndex] != 0) return { thisTS, at_unexpectedToken };
			unsigned int objectId = lutGetId(tokenIndex);
			thisTS->objectId.push_back(objectId);
			tokenIndex += 1;
			char *token = getToken(tokenIndex), *end;
			unsigned int objectSize = strtol(token, &end, 10);
			if (end - token != tokenEnd[tokenIndex] - tokenStart[tokenIndex] + 1) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			if (!checkForToken(tokenIndex, "{")) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			TreeStructreReturn newObject = create(thisTS, tst_objectType, true, false, true, false, true, objectSize, false, true, objectId);
			if (newObject.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->object.push_back(newObject.ts);
		}
		else if (checkForToken(tokenIndex, "nativeFunction"))
		{
			tokenIndex += 1;
			if (!isTokenObjectType(tokenIndex, thisTS)) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			if (tokenType[tokenIndex] != 0 || (tokenType[tokenIndex] != 3 && thisTS->type == tst_objectType)) return { thisTS, at_unexpectedToken };
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
			TreeStructreReturn newFunction = create(thisTS, tst_scopeFunction, true, false, true, true, false, Interpreter::Thread::ScopeMinSize, true, false, functionId);
			if (newFunction.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->function.push_back(newFunction.ts);
			newFunction.ts->codeRoot->addToCode({ Interpreter::Thread::cmd_callNative, nativeFunctionIndex });
			if (!checkForToken(tokenIndex, ";")) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
		}
		else if (checkForToken(tokenIndex, "name"))
		{
			tokenIndex += 1;
			if (tokenType[tokenIndex] != 0) return { thisTS, at_unexpectedToken };
			unsigned int nameId = lutGetId(tokenIndex);
			thisTS->nameId.push_back(nameId);
			tokenIndex += 1;
			if (!checkForToken(tokenIndex, "{")) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			TreeStructreReturn newName = create(parent, tst_name, true, false, true, allowRunningCode, true, 0, false, true, nameId);
			if (newName.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->name.push_back(newName.ts);
		}
		else if (checkForToken(tokenIndex, "if"))
		{
			if (!allowRunningCode) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			std::vector<unsigned int*> statmentPointer;
			std::vector<unsigned int> statmentPosition;
			bool last = false;

			while (true)
			{
				if (!last)
				{
					if (!checkForToken(tokenIndex, "(")) return { thisTS, at_unexpectedToken };
					tokenIndex += 1;
					ActionType actionType = evaluateExpretion(thisTS, false, true, 1);
					if (actionType == at_unexpectedToken) return { thisTS, at_unexpectedToken };
				}

				TreeStructreReturn newScope;
				if (checkForToken(tokenIndex, "{")) { tokenIndex += 1; newScope = create(thisTS, tst_scope, true, false, true, true, false, Interpreter::Thread::ScopeMinSize, false, true); }
				else newScope = create(thisTS, tst_scope, true, true, true, true, false, 7, false, true);
				if (newScope.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };

				thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_checkJump, 3 });
				thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_newScope, newScope.ts->size, 0, newScope.ts->codeRoot->getCodeStart(&codePointerLut) });
				if (last) break;
				thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_jump, 0 });
				statmentPointer.push_back(&thisTS->codeRoot->code.back());
				statmentPosition.push_back(thisTS->codeRoot->code.size() - 1);

				if (!checkForToken(tokenIndex, "else")) break;
				if (checkForToken(tokenIndex + 1, "if")) tokenIndex += 2;
				else
				{
					last = true;
					tokenIndex += 1;
				}
			}

			for (unsigned int i = 0; i < statmentPointer.size(); i++)
			{
				*statmentPointer[i] = thisTS->codeRoot->code.size() - statmentPosition[i] + 1;
			}
		}
		else if (checkForToken(tokenIndex, "for"))
		{
			if (!allowRunningCode) return { thisTS, at_unexpectedToken };

			tokenIndex += 1;
			if (!checkForToken(tokenIndex, "(")) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			ActionType actionType = evaluateExpretion(thisTS, true, false);
			if (actionType == at_unexpectedToken) return { thisTS, at_unexpectedToken };

			unsigned int codeJumpPosition = thisTS->codeRoot->code.size();
			actionType = evaluateExpretion(thisTS, true, false, 1);
			if (actionType == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_checkJump, 0 });
			unsigned int* writeStatmentEnd = &thisTS->codeRoot->code.back();
			unsigned int position = thisTS->codeRoot->code.size() - 2;
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_newScope, 0, 0, 0 });

			actionType = evaluateExpretion(thisTS, false, true);
			if (actionType == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_jump, codeJumpPosition - thisTS->codeRoot->code.size() });
			*writeStatmentEnd = thisTS->codeRoot->code.size() - position;

			TreeStructreReturn newScope;
			if (checkForToken(tokenIndex, "{")) { tokenIndex += 1; newScope = create(thisTS, tst_scopeForLoop, true, false, true, true, false, Interpreter::Thread::ScopeMinSize, false, true); }
			else newScope = create(thisTS, tst_scopeForLoop, true, true, true, true, false, 7, false, true);
			if (newScope.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			writeStatmentEnd[1] = Interpreter::Thread::cmd_newScope;
			writeStatmentEnd[2] = newScope.ts->size;
			writeStatmentEnd[3] = 0;
			writeStatmentEnd[4] = newScope.ts->codeRoot->getCodeStart(&codePointerLut);
		}
		else if (checkForToken(tokenIndex, "while"))
		{
			if (!allowRunningCode) return { thisTS, at_unexpectedToken };

			tokenIndex += 1;
			if (!checkForToken(tokenIndex, "(")) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			unsigned int statmentStart = thisTS->codeRoot->code.size();
			ActionType actionType = evaluateExpretion(thisTS, true, false, 1);
			if (actionType == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_checkJump, 7 });

			TreeStructreReturn newScope;
			if (checkForToken(tokenIndex, "{")) { tokenIndex += 1; newScope = create(thisTS, tst_scopeWhileLoop, true, false, true, true, false, Interpreter::Thread::ScopeMinSize, false, true); }
			else newScope = create(thisTS, tst_scopeWhileLoop, true, true, true, true, false, 7, false, true);
			if (newScope.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_newScope, newScope.ts->size, 0, newScope.ts->codeRoot->getCodeStart(&codePointerLut) });
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_jump, statmentStart - thisTS->codeRoot->code.size() });
		}
		else if (checkForToken(tokenIndex, "break"))
		{
			if (!allowRunningCode) return { thisTS, at_unexpectedToken };

			unsigned int loopScopesBack = 1;
			bool forLoop = false;
			bool whileLoop = false;

			TreeStructure* currentTS = thisTS;

			while (true)
			{
				if (currentTS->type == tst_scopeForLoop) { forLoop = true; break; }
				if (currentTS->type == tst_scopeWhileLoop) { whileLoop = true; break; }
				if (currentTS->type == tst_scopeFunction) break;
				loopScopesBack++;
				if (!currentTS->hasParent) break;
				currentTS = currentTS->parent->codeRoot;
			}

			if (forLoop || whileLoop) thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_endScope, loopScopesBack, 2 });
			else return { thisTS, at_unexpectedToken };

			tokenIndex += 1;
			if (!checkForToken(tokenIndex, ";")) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
		}
		else if (checkForToken(tokenIndex, "continue"))
		{
			if (!allowRunningCode) return { thisTS, at_unexpectedToken };

			unsigned int loopScopesBack = 1;
			bool forLoop = false;
			bool whileLoop = false;

			TreeStructure* currentTS = thisTS;

			while (true)
			{
				if (currentTS->type == tst_scopeForLoop) { forLoop = true; break; }
				if (currentTS->type == tst_scopeWhileLoop) { whileLoop = true; break; }
				if (currentTS->type == tst_scopeFunction) break;
				loopScopesBack++;
				if (!currentTS->hasParent) break;
				currentTS = currentTS->parent->codeRoot;
			}

			if (forLoop || whileLoop) thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_endScope, loopScopesBack, 0 });
			else return { thisTS, at_unexpectedToken };

			tokenIndex += 1;
			if (!checkForToken(tokenIndex, ";")) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
		}
		else if (checkForToken(tokenIndex, "return"))
		{
			unsigned int functionScopesBack = 1;
			bool function = false;
			TreeStructure* currentTS = thisTS;

			while (true)
			{
				if (currentTS->type == tst_scopeFunction) { function = true; break; }
				functionScopesBack++;
				if (!currentTS->hasParent) break;
				currentTS = currentTS->parent->codeRoot;
			}

			if (function)
			{
				if (checkForToken(tokenIndex + 1, ";"))
				{
					if(currentTS->size == 0) thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_endScope, functionScopesBack, 0 });
				}
				else
				{
					ActionType actionType = evaluateExpretion(thisTS, true, false, currentTS->returnTypeId);
					if (actionType == at_unexpectedToken) return { thisTS, at_unexpectedToken };
					if (lastTypeId != currentTS->returnTypeId) return { thisTS, at_unexpectedToken };
					thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_returnValue, functionScopesBack });
				}
			}
			else return { thisTS, at_unexpectedToken };
		}
		else if (isTokenObjectType(tokenIndex, thisTS))
		{
			TreeStructure *object = lastToken;
			tokenIndex += lastTokenLength;

			if (checkForToken(tokenIndex + 1, "("))
			{
				if (tokenType[tokenIndex] != 0 || (tokenType[tokenIndex] != 3 && thisTS->type == tst_objectType)) return { thisTS, at_unexpectedToken };
				unsigned int functionId = lutGetId(tokenIndex);
				thisTS->functionId.push_back(functionId);
				tokenIndex += 1;
				TreeStructreReturn newFunction = create(thisTS, tst_scopeFunction, true, false, true, true, false, 7, true, true, functionId);
				if (newFunction.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
				thisTS->function.push_back(newFunction.ts);
			}
			else
			{
				if (object->size == 0) { tokenIndex -= lastTokenLength; return { thisTS, at_unexpectedToken }; }
				if (tokenType[tokenIndex] != 0) return { thisTS, at_unexpectedToken };
				unsigned int instanceId = lutGetId(tokenIndex);
				thisTS->instanceId.push_back(instanceId);
				thisTS->instanceType.push_back(object);
				if (checkForToken(tokenIndex + 1, "="))
				{
					ActionType actionType = evaluateExpretion(thisTS, true, false, 0);
					if (actionType == at_unexpectedToken) return { thisTS, at_unexpectedToken };
				}
				else tokenIndex += 1;
			}
		}
		else
		{
			ActionType actionType = evaluateExpretion(thisTS, true, false, 0);
			if (actionType == at_unexpectedToken) return { thisTS, at_unexpectedToken };
		}
	}

	if(!doLoop) return { thisTS, at_normal };
	return { thisTS, at_unexpectedToken };
}

Compiler::ActionType Compiler::evaluateExpretion(TreeStructure *parent, bool semicolonEnd, bool bracketEnd, unsigned int preferedType, unsigned int firstToken, unsigned int lastToken)
{
	return at_normal;
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
	lastTokenLength = 1;

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

	while(found)
	{
		found = false;

		if (!checkForToken(index + 1, ".")) return false;
		index += 2;
		lastTokenLength += 2;

		for (unsigned int objectIndex = 0; objectIndex < thisTS->objectId.size(); objectIndex++) if (checkForToken(index, lut[thisTS->objectId[objectIndex]]))
		{
			lastToken = thisTS->object[objectIndex];
			return true;
		}
		if (!found) for (unsigned int nameIndex = 0; nameIndex < thisTS->nameId.size(); nameIndex++) if (checkForToken(index, lut[thisTS->nameId[nameIndex]])) found = true;
		if (!found) for (unsigned int instanceIndex = 0; instanceIndex < thisTS->instanceId.size(); instanceIndex++) if (checkForToken(index, lut[thisTS->instanceId[instanceIndex]])) return false;
		if (!found) for (unsigned int functionIndex = 0; functionIndex < thisTS->functionId.size(); functionIndex++) if (checkForToken(index, lut[thisTS->functionId[functionIndex]])) return false;
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