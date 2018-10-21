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
	for (; index < codeLength; index++)
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

bool Compiler::compileCode(Interpreter *interpreter)
{
	TreeStructureReturn a = create(NULL, tst_scope, false, false, true, true, false, Interpreter::Thread::ScopeMinSize, false, true);
	if (a.at == at_unexpectedToken) return false;

	std::vector<unsigned int> *fullCode = new std::vector<unsigned int>();
	std::vector<unsigned int> *typeSizeLut = new std::vector<unsigned int>();

	fullCode->insert(fullCode->end(), a.ts->code.begin(), a.ts->code.end());

	for (unsigned int i = 0; i < codeTreeStruct.size(); i++)
	{
		codePointerLut[i] = fullCode->size();
		fullCode->insert(fullCode->end(), codeTreeStruct[i]->code.begin(), codeTreeStruct[i]->code.end());
	}

	for (unsigned int i = 0; i < typeIdLut.size(); i++) typeSizeLut->push_back(typeIdLut[i]->size);

	interpreter->codePointer = &fullCode->at(0);
	interpreter->function = nativeFunction;
	interpreter->codePointerLut = &codePointerLut[0];
	interpreter->typeSizeLut = &typeSizeLut->at(0);
	interpreter->literalPointer = &literalPointer[0];
	interpreter->literalTypeId = &literalTypeId[0];
	interpreter->globalSize = a.ts->size;

	return true;
}

Compiler::TreeStructureReturn Compiler::create(TreeStructure *parent, TreeStructureTypes tsType, bool hasParent, bool singleLine, bool allowInstances, bool allowRunningCode, bool writeCodeToRoot, unsigned int startingSize, bool takeParameters, bool doLoop, unsigned int lutId, unsigned int returnType, bool addAsObject)
{
	TreeStructure* thisTS = new TreeStructure{ tsType, startingSize, hasParent, parent };
	if (writeCodeToRoot) thisTS->codeRoot = parent->codeRoot;
	else thisTS->codeRoot = thisTS;
	thisTS->lutId = lutId;
	thisTS->returnTypeId = returnType;
	allTreeStructures.push_back(thisTS);
	if (addAsObject) parent->object.push_back(thisTS);

	if (takeParameters)
	{
		if (!checkForToken(tokenIndex, "(")) return { thisTS, at_unexpectedToken };
		tokenIndex += 1;
		bool defaulValue = false;
		unsigned int parameterPosition = Interpreter::Thread::ScopeMinSize;

		while (true)
		{
			if (!isTokenObjectType(tokenIndex, thisTS)) return { thisTS, at_unexpectedToken };
			TreeStructure *object = lastToken;
			tokenIndex += lastTokenLength;
			if (object->size == 0) { tokenIndex -= lastTokenLength; return { thisTS, at_unexpectedToken }; }
			if (tokenType[tokenIndex] != 0) return { thisTS, at_unexpectedToken };
			unsigned int instanceId = lutGetId(tokenIndex);
			thisTS->instanceId.push_back(instanceId);
			thisTS->instanceType.push_back(object);
			thisTS->variableType.push_back(typeLutGetId(object));
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_copyParameter, parameterPosition, 0 });
			parameterPosition += object->size;
			if (checkForToken(tokenIndex + 1, "="))
			{
				ExprStructReturn exprStructReturn = evaluateExpretion(thisTS, ",)", 0);
				if (exprStructReturn.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
				defaulValue = true;
				thisTS->maxVariables += 1;
				if (checkForToken(tokenIndex - 1, ")"))
				{
					break;
				}
			}
			else if (checkForToken(tokenIndex + 1, ","))
			{
				if(defaulValue == true) return { thisTS, at_unexpectedToken };
				tokenIndex += 1;
				thisTS->minVariables += 1;
				thisTS->maxVariables += 1;
			}
			else if (checkForToken(tokenIndex + 1, ")"))
			{
				if (defaulValue == true) return { thisTS, at_unexpectedToken };
				thisTS->minVariables += 1;
				thisTS->maxVariables += 1;
				tokenIndex += 2;
				break;
			}
			else return { thisTS, at_unexpectedToken };
		}

		if (!checkForToken(tokenIndex, "{") && doLoop) return { thisTS, at_unexpectedToken };
		if (doLoop) tokenIndex += 1;
	}

	while (tokenIndex < tokenStart.size() && doLoop)
	{
		if (checkForToken(tokenIndex, "{"))
		{
			if (!allowRunningCode) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			TreeStructureReturn newScope = create(thisTS, tst_scope, true, false, true, true, false, Interpreter::Thread::ScopeMinSize, false, true);
			if (newScope.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_newScope, newScope.ts->size, 0, newScope.ts->codeRoot->getCodeStart(&codePointerLut, &codeTreeStruct), 1 });
		}
		else if (checkForToken(tokenIndex, "}"))
		{
			if (singleLine) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			if (thisTS->type == tst_scopeFunction && !thisTS->hasReturn && typeIdLut[thisTS->returnTypeId]->size > 0) return { thisTS, at_unexpectedToken };
			if (allowRunningCode) thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_endScope, 1 });
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
			TreeStructureReturn newObject = create(thisTS, tst_objectType, true, false, true, false, true, 0, false, true, objectId, 0, true);
			if (newObject.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			//thisTS->object.push_back(newObject.ts);
			typeLutGetId(newObject.ts);
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
			free(token);
			tokenIndex += 1;
			if (!checkForToken(tokenIndex, "{")) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			TreeStructureReturn newObject = create(thisTS, tst_objectType, true, false, true, false, true, objectSize, false, true, objectId, 0, true);
			if (newObject.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			//thisTS->object.push_back(newObject.ts);
			typeLutGetId(newObject.ts);
		}
		else if (checkForToken(tokenIndex, "nativeFunction"))
		{
			tokenIndex += 1;
			if (!isTokenObjectType(tokenIndex, thisTS)) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			if (tokenType[tokenIndex] != 0 && (tokenType[tokenIndex] != 3 && thisTS->type == tst_objectType)) return { thisTS, at_unexpectedToken };
			bool isOperator = false;
			if (tokenType[tokenIndex] == 3) isOperator = true;
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
			TreeStructureReturn newFunction = create(thisTS, tst_scopeFunction, true, false, true, true, false, Interpreter::Thread::ScopeMinSize, true, false, functionId);
			if (newFunction.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->function.push_back(newFunction.ts);
			if (isOperator && newFunction.ts->maxVariables > 1) return { thisTS, at_unexpectedToken };
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
			TreeStructureReturn newName = create(parent, tst_name, true, false, true, allowRunningCode, true, 0, false, true, nameId);
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
					ExprStructReturn exprStructReturn = evaluateExpretion(thisTS, ")", 1);
					if (exprStructReturn.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
					if (exprStructReturn.returnTypeId != 1) return { thisTS, at_unexpectedToken };
				}

				TreeStructureReturn newScope;
				if (checkForToken(tokenIndex, "{")) { tokenIndex += 1; newScope = create(thisTS, tst_scope, true, false, true, true, false, Interpreter::Thread::ScopeMinSize, false, true); }
				else newScope = create(thisTS, tst_scope, true, true, true, true, false, 7, false, true);
				if (newScope.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };

				thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_checkJump, 3 });
				thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_newScope, newScope.ts->size, 0, newScope.ts->codeRoot->getCodeStart(&codePointerLut, &codeTreeStruct), 1 });
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
			if(!isTokenObjectType(tokenIndex, thisTS)) return { thisTS, at_unexpectedToken };
			TreeStructure *object = lastToken;
			tokenIndex += lastTokenLength;
			if (object->size == 0) { tokenIndex -= lastTokenLength; return { thisTS, at_unexpectedToken }; }
			if (tokenType[tokenIndex] != 0) return { thisTS, at_unexpectedToken };
			unsigned int instanceId = lutGetId(tokenIndex);
			thisTS->instanceId.push_back(instanceId);
			thisTS->instanceType.push_back(object);
			if (checkForToken(tokenIndex + 1, "="))
			{
				ExprStructReturn exprStructReturn = evaluateExpretion(thisTS, ";", 0);
				if (exprStructReturn.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			}
			else tokenIndex += 1;

			unsigned int codeJumpPosition = thisTS->codeRoot->code.size();
			ExprStructReturn exprStructReturn = evaluateExpretion(thisTS, ";", 1);
			if (exprStructReturn.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			if (exprStructReturn.returnTypeId != 1) return { thisTS, at_unexpectedToken };
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_checkJump, 0 });
			unsigned int* writeStatmentEnd = &thisTS->codeRoot->code.back();
			unsigned int position = thisTS->codeRoot->code.size() - 2;
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_newScope, 0, 0, 0, 1 });

			exprStructReturn = evaluateExpretion(thisTS, ")", 0);
			if (exprStructReturn.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_jump, codeJumpPosition - thisTS->codeRoot->code.size() });
			*writeStatmentEnd = thisTS->codeRoot->code.size() - position;

			TreeStructureReturn newScope;
			if (checkForToken(tokenIndex, "{")) { tokenIndex += 1; newScope = create(thisTS, tst_scopeForLoop, true, false, true, true, false, Interpreter::Thread::ScopeMinSize, false, true); }
			else newScope = create(thisTS, tst_scopeForLoop, true, true, true, true, false, 7, false, true);
			if (newScope.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			writeStatmentEnd[1] = Interpreter::Thread::cmd_newScope;
			writeStatmentEnd[2] = newScope.ts->size;
			writeStatmentEnd[3] = 0;
			writeStatmentEnd[4] = newScope.ts->codeRoot->getCodeStart(&codePointerLut, &codeTreeStruct);
		}
		else if (checkForToken(tokenIndex, "while"))
		{
			if (!allowRunningCode) return { thisTS, at_unexpectedToken };

			tokenIndex += 1;
			if (!checkForToken(tokenIndex, "(")) return { thisTS, at_unexpectedToken };
			tokenIndex += 1;
			unsigned int statmentStart = thisTS->codeRoot->code.size();
			ExprStructReturn exprStructReturn = evaluateExpretion(thisTS, ")", 1);
			if (exprStructReturn.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			if (exprStructReturn.returnTypeId != 1) return { thisTS, at_unexpectedToken };
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_checkJump, 7 });

			TreeStructureReturn newScope;
			if (checkForToken(tokenIndex, "{")) { tokenIndex += 1; newScope = create(thisTS, tst_scopeWhileLoop, true, false, true, true, false, Interpreter::Thread::ScopeMinSize, false, true); }
			else newScope = create(thisTS, tst_scopeWhileLoop, true, true, true, true, false, 7, false, true);
			if (newScope.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_newScope, newScope.ts->size, 0, newScope.ts->codeRoot->getCodeStart(&codePointerLut, &codeTreeStruct), 1 });
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
			thisTS->hasReturn = true;
			unsigned int functionScopesBack = 1;
			bool function = false;
			TreeStructure* currentTS = thisTS;
			tokenIndex += 1;

			while (true)
			{
				if (currentTS->type == tst_scopeFunction) { function = true; break; }
				functionScopesBack++;
				if (!currentTS->hasParent) break;
				currentTS = currentTS->parent->codeRoot;
			}

			if (function)
			{
				if (checkForToken(tokenIndex, ";"))
				{
					if(currentTS->size == 0) thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_endScope, functionScopesBack, 0 });
					else return { thisTS, at_unexpectedToken };
				}
				else
				{
					ExprStructReturn exprStructReturn = evaluateExpretion(thisTS, ";", currentTS->returnTypeId);
					if (exprStructReturn.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
					if (exprStructReturn.returnTypeId != currentTS->returnTypeId) return { thisTS, at_unexpectedToken };
					thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_returnValue, functionScopesBack });
				}
			}
			else return { thisTS, at_unexpectedToken };
		}
		else if (isTokenObjectType(tokenIndex, thisTS))
		{
			TreeStructure *object = lastToken;
			tokenIndex += lastTokenLength;

			if (isTokenFunction(tokenIndex, thisTS, true))
			{
				tokenIndex += lastTokenLength;
				if (lastToken->functionInitilized == true) return { thisTS, at_unexpectedToken };
				TreeStructure* lastTokenS = lastToken, *lastParentS = lastParent;
				TreeStructureReturn newFunction = create(lastParentS, tst_scopeFunction, true, false, true, true, false, 7, true, true, lastTokenS->lutId, lastTokenS->returnTypeId);
				if (newFunction.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
				newFunction.ts->functionInitilized = true;
				bool variablesMatch = true;
				for (unsigned int i = 0; i < lastTokenS->variableType.size(); i++) if (newFunction.ts->variableType[i] != lastTokenS->variableType[i]) variablesMatch = false;
				if (lastTokenS->minVariables != newFunction.ts->minVariables || lastTokenS->maxVariables != newFunction.ts->maxVariables || !variablesMatch) return { thisTS, at_unexpectedToken };

				if (lastTokenS->codeLutIndex != UINT32_MAX)
				{
					codeTreeStruct[lastTokenS->codeLutIndex] = newFunction.ts;
					newFunction.ts->codeLutIndex = lastTokenS->codeLutIndex;
				}
				lastParentS->function[lastIndex] = newFunction.ts;
			}
			else if (checkForToken(tokenIndex + 1, "("))
			{
				if (tokenType[tokenIndex] != 0 && (tokenType[tokenIndex] != 3 && thisTS->type == tst_objectType)) return { thisTS, at_unexpectedToken };
				bool isOperator = false;
				if (tokenType[tokenIndex] == 3) isOperator = true;
				unsigned int functionId = lutGetId(tokenIndex);
				thisTS->functionId.push_back(functionId);
				tokenIndex += 1;

				unsigned int paranthesis = 1, parameterStart = tokenIndex;
				if (!checkForToken(tokenIndex, "(")) return { thisTS, at_unexpectedToken };
				tokenIndex += 1;
				while (paranthesis != 0)
				{
					if (checkForToken(tokenIndex, "(")) paranthesis++;
					if (checkForToken(tokenIndex, ")")) paranthesis--;
					tokenIndex += 1;
				}

				if (checkForToken(tokenIndex, ";"))
				{
					tokenIndex = parameterStart;
					TreeStructureReturn newFunction = create(thisTS, tst_scopeFunction, true, false, true, true, false, Interpreter::Thread::ScopeMinSize, true, false, functionId, typeLutGetId(object));
					if (newFunction.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
					thisTS->function.push_back(newFunction.ts);
					if (isOperator && newFunction.ts->maxVariables > 1) return { thisTS, at_unexpectedToken };
					tokenIndex += 1;
				}
				else if (checkForToken(tokenIndex, "{"))
				{
					tokenIndex = parameterStart;
					TreeStructureReturn newFunction = create(thisTS, tst_scopeFunction, true, false, true, true, false, Interpreter::Thread::ScopeMinSize, true, true, functionId, typeLutGetId(object));
					if (newFunction.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
					thisTS->function.push_back(newFunction.ts);
					newFunction.ts->functionInitilized = true;
					if (isOperator && newFunction.ts->maxVariables > 1) return { thisTS, at_unexpectedToken };
				}
				else return { thisTS, at_unexpectedToken };
			}
			else
			{
				if (!allowInstances) return { thisTS, at_unexpectedToken };
				if (object->size == 0) { tokenIndex -= lastTokenLength; return { thisTS, at_unexpectedToken }; }
				if (tokenType[tokenIndex] != 0) return { thisTS, at_unexpectedToken };
				unsigned int instanceId = lutGetId(tokenIndex);
				thisTS->instanceId.push_back(instanceId);
				thisTS->nameHost.push_back(thisTS);
				thisTS->instanceType.push_back(object);
				thisTS->size += object->size;

				if (thisTS->type == tst_name)
				{
					thisTS->codeRoot->instanceId.push_back(instanceId);
					thisTS->codeRoot->nameHost.push_back(thisTS);
					thisTS->codeRoot->instanceType.push_back(object);
					thisTS->codeRoot->size += object->size;
				}

				if (checkForToken(tokenIndex + 1, "="))
				{
					ExprStructReturn exprStructReturn = evaluateExpretion(thisTS, ";", 0);
					if (exprStructReturn.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
				}
				else if (checkForToken(tokenIndex + 1, ";")) tokenIndex += 1;
				else return { thisTS, at_unexpectedToken };
			}
		}
		else
		{
			ExprStructReturn exprStructReturn = evaluateExpretion(thisTS, ";", 0);
			if (exprStructReturn.at == at_unexpectedToken) return { thisTS, at_unexpectedToken };
		}
	}

	if(!doLoop || tokenIndex >= tokenStart.size()) return { thisTS, at_normal };
	return { thisTS, at_unexpectedToken };
}

Compiler::ExprStructReturn Compiler::evaluateExpretion(TreeStructure *thisTS, const char* end, unsigned int preferedType)
{
	unsigned int firstExprToken = tokenIndex, lastExprToken, endSize = strlen(end);
	bool foundEnd = false;
	while (!foundEnd)
	{
		for (unsigned int i = 0; i < endSize; i++) if (tokenStart[tokenIndex] == tokenEnd[tokenIndex] && rawCode[tokenStart[tokenIndex]] == end[i])
		{
			foundEnd = true;
			break;
		}
		tokenIndex += 1;
	}
	lastExprToken = tokenIndex - 2;

	std::vector<ExprStruct> expr;

	for (unsigned int i = firstExprToken; i <= lastExprToken;)
	{
		if (isTokenInstance(i, thisTS))
		{
			i += lastTokenLength;
			expr.push_back({ es_variable, scopesBack, indexFromThere, lastToken->instanceType[lastIndex] });
		}
		else if (isTokenFunction(i, thisTS, false))
		{
			i += lastTokenLength;
			expr.push_back({ es_function, 0, 0, lastToken });
			expr[expr.size() - 1].funcScopesBack = scopesBack;
		}
		else if (checkForToken(i, "false"))
		{
			expr.push_back({ es_literal, getLiteralId((char*)"\000", 1), 0, typeIdLut[1] });
			i += 1;
		}
		else if (checkForToken(i, "true"))
		{
			expr.push_back({ es_literal, getLiteralId((char*)"\001", 1), 0, typeIdLut[1] });
			i += 1;
		}
		else if (tokenType[i] == 1)
		{
			char *token = getToken(i), *end;
			double number = strtod(token, &end);
			int intNumber = number;
			if (end - token != tokenEnd[i] - tokenStart[i] + 1) { tokenIndex = i; return { 0, at_unexpectedToken }; }
			free(token);
			if (number == intNumber) expr.push_back({ es_literal, getLiteralId((char*)&intNumber, 2), 0, typeIdLut[2] });
			else expr.push_back({ es_literal, getLiteralId((char*)&number, 3), 0, typeIdLut[3] });
			i += 1;
		}
		else if (tokenType[i] == 3)
		{
			expr.push_back({ es_operator, lutGetId(i), 0, 0 });
			i += 1;
		}
		else if (checkForToken(i, "("))
		{
			i += 1;

			if (isTokenObjectType(i, thisTS))
			{
				i += lastTokenLength;
				if (!checkForToken(i, ")")) { tokenIndex = i; return { 0, at_unexpectedToken }; }
				i += 1;
				expr.push_back({ es_operator, 0, 1, lastToken });
			}
			else
			{
				expr.push_back({ es_open, 0, 0, 0 });
			}
		}
		else if (checkForToken(i, ")"))
		{
			expr.push_back({ es_close, 0, 0, 0 });
			i += 1;
		}
		else if (checkForToken(i, ","))
		{
			expr.push_back({ es_seperator, 0, 0, 0 });
			i += 1;
		}
		else if (tokenType[i] == 4 || tokenType[i] == 5)
		{
			std::vector<char> stringLiteral;

			for (unsigned s = tokenStart[i] + 1; s < tokenEnd[i]; s++)
			{
				if (rawCode[s] == '\\')
				{
					if(s == tokenEnd[i] - 1) { tokenIndex = i; return { 0, at_unexpectedToken }; }

					if(rawCode[s + 1] == '\\') stringLiteral.push_back('\\');
					else if (rawCode[s + 1] == '0') stringLiteral.push_back('\0');
					else if (rawCode[s + 1] == 'n') stringLiteral.push_back('\n');
					else if (rawCode[s + 1] == '\'') stringLiteral.push_back('\'');
					else if (rawCode[s + 1] == '"') stringLiteral.push_back('"');

					s += 1;
				}
				else stringLiteral.push_back(rawCode[s]);
			}

			//TODO: strings

			i += 1;
		}
		else { tokenIndex = i; return { 0, at_unexpectedToken }; }
	}

	thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_clearWS });

	for (unsigned int i = 0; i < expr.size(); i++)
	{
		if (expr[i].type == es_variable || expr[i].type == es_literal || expr[i].type == es_stored || expr[i].type == es_function)
		{
			TreeStructure *type = expr[i].instanceType;
			if (expr[i].type == es_function) type = typeIdLut[expr[i].function->returnTypeId];

			while (i > 0)
			{
				if (expr[i - 1].type == es_operator)
				{
					bool found = false;
					for (unsigned int f = 0; f < type->function.size(); f++) if ((type->functionId[f] == expr[i - 1].lutId && expr[i - 1].operatorType == 0) || (typeIdLut[type->returnTypeId] == expr[i - 1].instanceType && expr[i - 1].operatorType == 1))
					{
						if (type->function[f]->maxVariables != 0) continue;
						if (expr[i - 1].operatorType == 0) { expr[i].unaryOperators.push_back(type->function[f]->lutId); expr[i].unaryIsCast.push_back(false); }
						else if (expr[i - 1].operatorType == 1) { expr[i].unaryOperators.push_back(type->returnTypeId); expr[i].unaryIsCast.push_back(true); }
						else return { NULL, at_unexpectedToken };
						type = typeIdLut[type->function[f]->returnTypeId];
						expr.erase(expr.begin() + i - 1);
						i -= 1;
						found = true;
					}
					if (found) continue;
				}
				break;
			}

			while (i < expr.size() - 1)
			{
				if (expr[i + 1].type == es_operator)
				{
					bool found = false;
					for (unsigned int f = 0; f < type->function.size(); f++) if ((type->functionId[f] == expr[i + 1].lutId && expr[i + 1].operatorType == 0) || (typeIdLut[type->returnTypeId] == expr[i + 1].instanceType && expr[i + 1].operatorType == 1))
					{
						if (type->function[f]->maxVariables != 0) continue;
						if (expr[i + 1].operatorType == 0) { expr[i].unaryOperators.push_back(type->function[f]->lutId); expr[i].unaryIsCast.push_back(false); }
						else if (expr[i + 1].operatorType == 1) { expr[i].unaryOperators.push_back(type->returnTypeId); expr[i].unaryIsCast.push_back(true); }
						else return { NULL, at_unexpectedToken };
						type = typeIdLut[type->function[f]->returnTypeId];
						expr.erase(expr.begin() + i + 1);
						found = true;
					}
					if (found) continue;

				}
				break;
			}

			expr[i].afterType = type;
		}
		else if (expr[i].type == es_open)
		{
			while (i > 1)
			{
				if (expr[i - 1].type == es_operator && expr[i - 2].type == es_operator)
				{
					if (expr[i - 1].operatorType == 0) { expr[i].unaryOperators.push_back(expr[i - 1].lutId); expr[i].unaryIsCast.push_back(false); }
					else if (expr[i - 1].operatorType == 1) { expr[i].unaryOperators.push_back(typeLutGetId(expr[i - 1].instanceType)); expr[i].unaryIsCast.push_back(true); }
					else return { NULL, at_unexpectedToken };
					expr.erase(expr.begin() + i - 1);
					i -= 1;
					continue;
				}
				break;
			}
		}
	}

	std::vector<unsigned int> parenthesesStart;

	for (unsigned int i = 0; i < expr.size(); i++)
	{
		if (expr[i].type == es_open) parenthesesStart.push_back(i);
		else if (expr[i].type == es_function)
		{
			parenthesesStart.push_back(i);
			i += 1;
		}
		else if (expr[i].type == es_close)
		{
			while (i < expr.size() - 2)
			{
				if (expr[i + 1].type == es_operator && expr[i + 2].type == es_operator)
				{
					if (expr[i + 1].operatorType == 0) { expr[parenthesesStart.back()].unaryOperators.push_back(expr[i + 1].lutId); expr[parenthesesStart.back()].unaryIsCast.push_back(false); }
					else if (expr[i + 1].operatorType == 1) { expr[parenthesesStart.back()].unaryOperators.push_back(typeLutGetId(expr[i + 1].instanceType)); expr[parenthesesStart.back()].unaryIsCast.push_back(true); }
					else return { NULL, at_unexpectedToken };
					expr.erase(expr.begin() + i + 1);
					parenthesesStart.pop_back();
					continue;
				}
				break;
			}
		}
	}

	TreeStructureReturn exprResult = evalExpr(thisTS, expr);
	if (exprResult.at == at_unexpectedToken) return { 0, at_unexpectedToken };

	if (exprResult.ts != typeIdLut[preferedType])
	{
		for (unsigned int f = 0; f < exprResult.ts->function.size(); f++) if (exprResult.ts->function[f]->returnTypeId == preferedType)
		{
			if (exprResult.ts->function[f]->maxVariables != 0) continue;
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_newScope, exprResult.ts->function[f]->size, 1, exprResult.ts->function[f]->getCodeStart(&codePointerLut, &codeTreeStruct) });
			return { preferedType, at_normal };
		}
	}

	return { typeLutGetId(exprResult.ts), at_normal };
}

Compiler::TreeStructureReturn Compiler::evalExpr(TreeStructure *thisTS, std::vector<ExprStruct> expr)
{
	if (expr.size() == 1)
	{
		if (expr[0].type == es_variable)
		{
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_push, expr[0].scopesBack, expr[0].indexFromThere, typeLutGetId(expr[0].instanceType) });
			TreeStructure *type = expr[0].instanceType;

			for (unsigned int i = 0; i < expr[0].unaryOperators.size(); i++)
			{
				TreeStructure *unaryOperator = NULL;
				for (unsigned int f = 0; f < type->function.size(); f++) if ((type->functionId[f] == expr[0].unaryOperators[i] && !expr[0].unaryIsCast[i]) || (type->returnTypeId == expr[0].unaryOperators[i] && expr[0].unaryIsCast[i]))
				{
					if (type->function[f]->maxVariables != 0) continue;
					unaryOperator = type->function[f];
					type = typeIdLut[unaryOperator->returnTypeId];
				}
				if (unaryOperator == NULL) return { NULL, at_unexpectedToken };
				thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_newScope, unaryOperator->size, 1, unaryOperator->getCodeStart(&codePointerLut, &codeTreeStruct) });
			}

			return { type, at_normal };
		}
		else if (expr[0].type == es_literal)
		{
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_pushLiteral, expr[0].literalId, typeLutGetId(expr[0].instanceType) });
			TreeStructure *type = expr[0].instanceType;

			for (unsigned int i = 0; i < expr[0].unaryOperators.size(); i++)
			{
				TreeStructure *unaryOperator = NULL;
				for (unsigned int f = 0; f < type->function.size(); f++) if ((type->functionId[f] == expr[0].unaryOperators[i] && !expr[0].unaryIsCast[i]) || (type->returnTypeId == expr[0].unaryOperators[i] && expr[0].unaryIsCast[i]))
				{
					if (type->function[f]->maxVariables != 0) continue;
					unaryOperator = type->function[f];
					type = typeIdLut[unaryOperator->returnTypeId];
				}
				if (unaryOperator == NULL) return { NULL, at_unexpectedToken };
				thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_newScope, unaryOperator->size, 1, unaryOperator->getCodeStart(&codePointerLut, &codeTreeStruct) });
			}

			return { type, at_normal };
		}
		else if (expr[0].type == es_stored)
		{
			return { expr[0].instanceType, at_normal };
		}
		else return { NULL, at_unexpectedToken };
	}
	else if (expr.front().type == es_open && expr.back().type == es_close)
	{
		TreeStructureReturn exprResult = evalExpr(thisTS, std::vector<ExprStruct>(&expr.front() + 1, &expr.back()));
		if (exprResult.at == at_unexpectedToken) return { NULL, at_unexpectedToken };

		TreeStructure *type = exprResult.ts;

		for (unsigned int i = 0; i < expr[0].unaryOperators.size(); i++)
		{
			TreeStructure *unaryOperator = NULL;
			for (unsigned int f = 0; f < type->function.size(); f++) if ((type->functionId[f] == expr[0].unaryOperators[i] && !expr[0].unaryIsCast[i]) || (type->returnTypeId == expr[0].unaryOperators[i] && expr[0].unaryIsCast[i]))
			{
				if (type->function[f]->maxVariables != 0) continue;
				unaryOperator = type->function[f];
				type = typeIdLut[unaryOperator->returnTypeId];
			}
			if (unaryOperator == NULL) return { NULL, at_unexpectedToken };
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_newScope, unaryOperator->size, 1, unaryOperator->getCodeStart(&codePointerLut, &codeTreeStruct) });
		}

		return { type, at_normal };
	}
	else if (expr.front().type == es_function)
	{
		if (expr[1].type != es_open) return { NULL, at_unexpectedToken };
		int parentheses = 1;
		unsigned int lastParameter = 2, parameterCount = 0, functionEnd = 0;
		for (unsigned int i = 2; i < expr.size(); i++)
		{
			if (expr[i].type == es_open) parentheses++;
			if (expr[i].type == es_close) parentheses--;
			if (parentheses < 0) return { NULL, at_unexpectedToken };
			if (expr[i].type == es_seperator || parentheses == 0)
			{
				if (parentheses > 1) continue;

				TreeStructureReturn exprParameterResult = evalExpr(thisTS, std::vector<ExprStruct>(&expr.front() + lastParameter, &expr.front() + i));
				if (exprParameterResult.at == at_unexpectedToken) return { NULL, at_unexpectedToken };
				if (typeIdLut[expr[0].function->variableType[parameterCount]] != exprParameterResult.ts) return { NULL, at_unexpectedToken };
				parameterCount++;
				lastParameter = i + 1;

				if (parentheses == 0)
				{
					functionEnd = i;
					break;
				}
			}
		}
		if (parentheses != 0) return { NULL, at_unexpectedToken };

		thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_newScope, expr[0].function->size, 2, expr[0].function->getCodeStart(&codePointerLut, &codeTreeStruct), expr[0].funcScopesBack });

		TreeStructure *type = typeIdLut[expr[0].function->returnTypeId];

		for (unsigned int i = 0; i < expr[0].unaryOperators.size(); i++)
		{
			TreeStructure *unaryOperator = NULL;
			for (unsigned int f = 0; f < type->function.size(); f++) if ((type->functionId[f] == expr[0].unaryOperators[i] && !expr[0].unaryIsCast[i]) || (type->returnTypeId == expr[0].unaryOperators[i] && expr[0].unaryIsCast[i]))
			{
				if (type->function[f]->maxVariables != 0) continue;
				unaryOperator = type->function[f];
				type = typeIdLut[unaryOperator->returnTypeId];
			}
			if (unaryOperator == NULL) return { NULL, at_unexpectedToken };
			thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_newScope, unaryOperator->size, 1, unaryOperator->getCodeStart(&codePointerLut, &codeTreeStruct) });
		}

		expr[functionEnd] = { es_stored, 0, 0, type };

		TreeStructureReturn exprResult = evalExpr(thisTS, std::vector<ExprStruct>(&expr.front() + functionEnd, &expr.back() + 1));
		if (exprResult.at == at_unexpectedToken) return { NULL, at_unexpectedToken };

		return { exprResult.ts, at_normal };
	}
	else
	{
		unsigned int bestOperator;
		char operatorLevel = -1;
		int parentheses = 0;
		for (unsigned int i = 0; i < expr.size(); i++)
		{
			if (expr[i].type == es_open) parentheses++;
			if (expr[i].type == es_close) parentheses--;
			if (parentheses < 0) return { NULL, at_unexpectedToken };
			if (parentheses == 0 && expr[i].type == es_operator && expr[i].operatorType != 1)
			{
				char thisOparatorLevel = 1;
				if (strcmp(lut[expr[i].lutId], "=") == 0 || strcmp(lut[expr[i].lutId], "+=") == 0 || strcmp(lut[expr[i].lutId], "-=") == 0 || strcmp(lut[expr[i].lutId], "*=") == 0 || strcmp(lut[expr[i].lutId], "/=") == 0) thisOparatorLevel = 0;
				else if (strcmp(lut[expr[i].lutId], "&&") == 0 || strcmp(lut[expr[i].lutId], "||") == 0) thisOparatorLevel = 3;
				else if (strcmp(lut[expr[i].lutId], "==") == 0 || strcmp(lut[expr[i].lutId], "!=") == 0 || strcmp(lut[expr[i].lutId], "<") == 0 || strcmp(lut[expr[i].lutId], ">") == 0 || strcmp(lut[expr[i].lutId], "<=") == 0 || strcmp(lut[expr[i].lutId], ">=") == 0) thisOparatorLevel = 2;
				else if (strcmp(lut[expr[i].lutId], "*") == 0 || strcmp(lut[expr[i].lutId], "/") == 0) thisOparatorLevel = 0;
				if (thisOparatorLevel > operatorLevel)
				{
					bestOperator = i;
					operatorLevel = thisOparatorLevel;
				}
			}
		}
		parentheses = 0;
		for (unsigned int i = expr.size() - 1; i > 0; i--)
		{
			if (expr[i].type == es_close) parentheses++;
			if (expr[i].type == es_open) parentheses--;
			if (parentheses < 0) return { NULL, at_unexpectedToken };
			if (parentheses == 0 && expr[i].type == es_operator && expr[i].operatorType != 1)
			{
				char thisOparatorLevel = 1;
				if (strcmp(lut[expr[i].lutId], "=") == 0 || strcmp(lut[expr[i].lutId], "+=") == 0 || strcmp(lut[expr[i].lutId], "-=") == 0 || strcmp(lut[expr[i].lutId], "*=") == 0 || strcmp(lut[expr[i].lutId], "/=") == 0) thisOparatorLevel = 4;
				else if (strcmp(lut[expr[i].lutId], "&&") == 0 || strcmp(lut[expr[i].lutId], "||") == 0) thisOparatorLevel = 3;
				else if (strcmp(lut[expr[i].lutId], "==") == 0 || strcmp(lut[expr[i].lutId], "!=") == 0 || strcmp(lut[expr[i].lutId], "<") == 0 || strcmp(lut[expr[i].lutId], ">") == 0 || strcmp(lut[expr[i].lutId], "<=") == 0 || strcmp(lut[expr[i].lutId], ">=") == 0) thisOparatorLevel = 2;
				else if (strcmp(lut[expr[i].lutId], "*") == 0 || strcmp(lut[expr[i].lutId], "/") == 0) thisOparatorLevel = 0;
				if (thisOparatorLevel > operatorLevel)
				{
					bestOperator = i;
					operatorLevel = thisOparatorLevel;
				}
			}
		}
		if (operatorLevel == -1) return { NULL, at_unexpectedToken };
		if (bestOperator == 0 || bestOperator == expr.size() - 1) return { NULL, at_unexpectedToken };
		if (expr[bestOperator - 1].type == es_operator || expr[bestOperator + 1].type == es_operator) return { NULL, at_unexpectedToken };

		TreeStructureReturn expr1Result = evalExpr(thisTS, std::vector<ExprStruct>(&expr.front(), &expr.front() + bestOperator));
		TreeStructureReturn expr2Result = evalExpr(thisTS, std::vector<ExprStruct>(&expr.front() + bestOperator + 1, &expr.back() + 1));
		if (expr1Result.at == at_unexpectedToken || expr2Result.at == at_unexpectedToken) return { NULL, at_unexpectedToken };

		TreeStructure *operatorFunction = NULL;
		for (unsigned int i = 0; i < expr1Result.ts->function.size(); i++) if (expr1Result.ts->functionId[i] == expr[bestOperator].lutId) operatorFunction = expr1Result.ts->function[i];
		if (operatorFunction == NULL) return { NULL, at_unexpectedToken };
		if (typeIdLut[operatorFunction->variableType[0]] != expr2Result.ts) return { NULL, at_unexpectedToken };

		unsigned int parentIsScopesBack = 0;
		TreeStructure *currentParent = thisTS;
		bool found = false;
		if (expr1Result.ts->parent == currentParent) found = true;
		while (currentParent->hasParent && !found)
		{
			if (expr1Result.ts->parent != currentParent->parent)
			{
				currentParent = currentParent->parent;
				parentIsScopesBack++;
			}
			else
			{
				found = true;
				break;
			}
		}
		if(!found) return { NULL, at_unexpectedToken };

		thisTS->codeRoot->addToCode({ Interpreter::Thread::cmd_newScope, operatorFunction->size, 2, operatorFunction->getCodeStart(&codePointerLut, &codeTreeStruct), parentIsScopesBack });
		return { typeIdLut[operatorFunction->returnTypeId], at_normal };
	}

	return { NULL, at_unexpectedToken };
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
		if (!found) for (unsigned int nameIndex = 0; nameIndex < thisTS->nameId.size(); nameIndex++) if (checkForToken(index, lut[thisTS->nameId[nameIndex]]))
		{
			found = true;
			thisTS = thisTS->name[nameIndex];
		}
		if (!found) for (unsigned int instanceIndex = 0; instanceIndex < thisTS->instanceId.size(); instanceIndex++) if (checkForToken(index, lut[thisTS->instanceId[instanceIndex]])) return false;
		if (!found) for (unsigned int functionIndex = 0; functionIndex < thisTS->functionId.size(); functionIndex++) if (checkForToken(index, lut[thisTS->functionId[functionIndex]])) return false;
	}

	return false;
}

bool Compiler::isTokenInstance(unsigned int index, TreeStructure *thisTS)
{
	bool found = false;
	lastTokenLength = 1;
	scopesBack = 0;

	while (!found)
	{
		indexFromThere = Interpreter::Thread::ScopeMinSize;

		for (unsigned int objectIndex = 0; objectIndex < thisTS->objectId.size(); objectIndex++) if (checkForToken(index, lut[thisTS->objectId[objectIndex]])) return false;
		if (!found) for (unsigned int nameIndex = 0; nameIndex < thisTS->nameId.size(); nameIndex++) if (checkForToken(index, lut[thisTS->nameId[nameIndex]])) found = true;
		if (!found) for (unsigned int instanceIndex = 0; instanceIndex < thisTS->instanceId.size(); instanceIndex++)
		{
			if (checkForToken(index, lut[thisTS->instanceId[instanceIndex]]))
			{
				lastToken = thisTS;
				lastIndex = instanceIndex;

				return true;
			}

			indexFromThere += thisTS->instanceType[instanceIndex]->size;
		}
		if (!found) for (unsigned int functionIndex = 0; functionIndex < thisTS->functionId.size(); functionIndex++) if (checkForToken(index, lut[thisTS->functionId[functionIndex]])) return false;

		if (!thisTS->hasParent) break;
		if (!found) { thisTS = thisTS->parent; scopesBack++; }
	}

	TreeStructure* truePosition = thisTS;
	lastToken = thisTS;

	while (found)
	{
		found = false;

		if (!checkForToken(index + 1, ".")) return false;
		index += 2;
		lastTokenLength += 2;

		for (unsigned int objectIndex = 0; objectIndex < thisTS->objectId.size(); objectIndex++) if (checkForToken(index, lut[thisTS->objectId[objectIndex]])) return false;
		if (!found) for (unsigned int nameIndex = 0; nameIndex < thisTS->nameId.size(); nameIndex++) if (checkForToken(index, lut[thisTS->nameId[nameIndex]]))
		{
			found = true;
			thisTS = thisTS->name[nameIndex];
		}
		if (!found) for (unsigned int instanceIndex = 0; instanceIndex < thisTS->instanceId.size(); instanceIndex++) if (checkForToken(index, lut[thisTS->instanceId[instanceIndex]]))
		{
			break;
		}
		if (!found) for (unsigned int functionIndex = 0; functionIndex < thisTS->functionId.size(); functionIndex++) if (checkForToken(index, lut[thisTS->functionId[functionIndex]])) return false;
	}

	indexFromThere = Interpreter::Thread::ScopeMinSize;

	for (unsigned int instanceIndex = 0; instanceIndex < truePosition->instanceId.size(); instanceIndex++)
	{
		if (checkForToken(index, lut[truePosition->instanceId[instanceIndex]]) && truePosition->nameHost[instanceIndex] == thisTS)
		{
			lastIndex = instanceIndex;

			return true;
		}

		indexFromThere += truePosition->instanceType[instanceIndex]->size;
	}

	return false;
}

bool Compiler::isTokenFunction(unsigned int index, TreeStructure *thisTS, bool includeOperators)
{
	bool found = false;
	lastTokenLength = 1;
	scopesBack = 0;

	while (!found)
	{
		for (unsigned int objectIndex = 0; objectIndex < thisTS->objectId.size(); objectIndex++) if (checkForToken(index, lut[thisTS->objectId[objectIndex]])) return false;
		if (!found) for (unsigned int nameIndex = 0; nameIndex < thisTS->nameId.size(); nameIndex++) if (checkForToken(index, lut[thisTS->nameId[nameIndex]])) found = true;
		if (!found) for (unsigned int instanceIndex = 0; instanceIndex < thisTS->instanceId.size(); instanceIndex++) if (checkForToken(index, lut[thisTS->instanceId[instanceIndex]])) return false;
		if (!found) for (unsigned int functionIndex = 0; functionIndex < thisTS->functionId.size(); functionIndex++) if (checkForToken(index, lut[thisTS->functionId[functionIndex]]))
		{
			if (tokenType[index] != 0 && !includeOperators) continue;
			lastToken = thisTS->function[functionIndex];
			lastParent = thisTS;
			lastIndex = functionIndex;
			return true;
		}

		if (!thisTS->hasParent) break;
		if (!found) { thisTS = thisTS->parent; scopesBack++; }
	}

	while (found)
	{
		found = false;

		if (!checkForToken(index + 1, ".")) return false;
		index += 2;
		lastTokenLength += 2;

		for (unsigned int objectIndex = 0; objectIndex < thisTS->objectId.size(); objectIndex++) if (checkForToken(index, lut[thisTS->objectId[objectIndex]])) return false;
		if (!found) for (unsigned int nameIndex = 0; nameIndex < thisTS->nameId.size(); nameIndex++) if (checkForToken(index, lut[thisTS->nameId[nameIndex]]))
		{
			found = true;
			thisTS = thisTS->name[nameIndex];
		}
		if (!found) for (unsigned int instanceIndex = 0; instanceIndex < thisTS->instanceId.size(); instanceIndex++) if (checkForToken(index, lut[thisTS->instanceId[instanceIndex]])) return false;
		if (!found) for (unsigned int functionIndex = 0; functionIndex < thisTS->functionId.size(); functionIndex++) if (checkForToken(index, lut[thisTS->functionId[functionIndex]]))
		{
			if (tokenType[index] != 0 && !includeOperators) continue;
			lastToken = thisTS->function[functionIndex];
			lastParent = thisTS;
			lastIndex = functionIndex;
			return true;
		}
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

void Compiler::addNativeFunction(const char* name, void(*function)(uintptr_t scope))
{
	if (nativeFunctionCount == 0)
	{
		nativeFunction = (void(**)(uintptr_t scope))malloc(4);
		nativeFunctionName = (char**)malloc(4);
	}
	else
	{
		nativeFunction = (void(**)(uintptr_t scope))realloc(nativeFunction, nativeFunctionCount * 4 + 4);
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
		for (unsigned int i = 0; lut[lutIndex][i] != '\0' && tokenStart[index] + i <= tokenEnd[index]; i++)
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

unsigned int Compiler::typeLutGetId(TreeStructure* type)
{
	for (unsigned int lutIndex = 0; lutIndex < typeIdLut.size(); lutIndex++) if (typeIdLut[lutIndex] == type) return lutIndex;
	typeIdLut.push_back(type);
	return typeIdLut.size() - 1;
}

unsigned int Compiler::getLiteralId(char* data, unsigned int typeId)
{
	unsigned int size = typeIdLut[typeId]->size;
	for (unsigned int i = 0; i < literalTypeId.size(); i++) if (literalTypeId[i] == typeId)
	{
		bool isSame = true;
		for (unsigned int d = 0; d < size; d++) if (((char*)literalPointer[i])[d] != data[d]) { isSame = false; break; }
		if (isSame) return i;
	}

	char* newLiteral = (char*)malloc(size);
	for (unsigned int i = 0; i < size; i++) newLiteral[i] = data[i];
	literalPointer.push_back((uintptr_t)newLiteral);
	literalTypeId.push_back(typeId);
	return literalPointer.size() - 1;
}