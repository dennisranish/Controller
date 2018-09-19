#include "Interpreter.h"

Interpreter::Interpreter()
{
}

void Interpreter::setProgram(char* programCode)
{
	parseCode(programCode);
	
	lookupTable = (unsigned int*)malloc(400);
	lookupTableType = (byte*)malloc(100);
	lookupTableReserved = 100;

	compileCodeIndex = 0;
	topCodeScope = createCodeScope(false);

	lookupTable = (unsigned int*)realloc(lookupTable, lookupTableCount * 4);
	lookupTableType = (byte*)realloc(lookupTableType, lookupTableCount);
}

void Interpreter::parseCode(char* code)
{
	programCode = code;
	
	tokenStart = (unsigned int*)malloc(400);
	tokenEnd = (unsigned int*)malloc(400);

	tokenCount = 0;
	unsigned int reservedTokenCount = 100;

	unsigned int characterType = 4;

	for(unsigned int i = 0; true; i++)
	{
		if(programCode[i] == '\0')
		{
			tokenEnd[tokenCount - 1] = i - 1;
			break;
		}
		
		unsigned int newCharacterType = 0;
		char a = programCode[i];
		char b = 0;
		char c = 0;

		if(i >= 1) b = programCode[i - 1];
		if(i >= 2) c = programCode[i - 2];

		//Comments
		if(a == '/' && programCode[i + 1] == '/' && characterType > 1 && characterType != 2 && characterType != 3) newCharacterType = 0;
		else if(a == '/' && programCode[i + 1] == '*' && characterType > 1 && characterType != 2 && characterType != 3) newCharacterType = 1;
		else if(characterType == 0 && b != '\n') newCharacterType = 0;
		else if(characterType == 1 && (c != '*' || b != '/')) newCharacterType = 1;

		//Strings
		else if(a == '"') newCharacterType = 2;
		else if(characterType == 2 && (b != '"' || c == '\\' || (i - tokenStart[tokenCount - 1]) == 1)) newCharacterType = 2;
		else if(a == '\'') newCharacterType = 3;
		else if(characterType == 3 && (b != '\'' || c == '\\' || (i - tokenStart[tokenCount - 1]) == 1)) newCharacterType = 3;

		//Code
		else if(a == ' ' || a == '\n' || a == '\t') newCharacterType = 4;
		else if(a == '{' || a == '}' || a == '(' || a == ')' || a == '[' || a == ']' || a == '.' || a == ';') newCharacterType = 5;
		else if((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || (a >= '0' && a <= '9') || a == '_') newCharacterType = 6;
		else newCharacterType = 7;

		if(newCharacterType != characterType || newCharacterType == 5)
		{
			if(tokenCount > 0 && (characterType > 4 || characterType == 2 || characterType == 3)) tokenEnd[tokenCount - 1] = i - 1;
			if(newCharacterType > 4 || newCharacterType == 2 || newCharacterType == 3)
			{
				if(tokenCount >= reservedTokenCount)
				{
					tokenStart = (unsigned int*)realloc(tokenStart, reservedTokenCount * 4 + 400);
					tokenEnd = (unsigned int*)realloc(tokenEnd, reservedTokenCount * 4 + 400);

					reservedTokenCount += 100;
				}
				
				tokenCount++;
				tokenStart[tokenCount - 1] = i;
			}

			characterType = newCharacterType;
		}
	}

	tokenStart = (unsigned int*)realloc(tokenStart, tokenCount * 4);
	tokenEnd = (unsigned int*)realloc(tokenEnd, tokenCount * 4);
}

char* Interpreter::getToken(unsigned int index)
{	
	char* tokenValue = (char*)malloc((tokenEnd[index] - tokenStart[index]) + 2);

	for(int i = 0; i <= tokenEnd[index] - tokenStart[index]; i++) tokenValue[i] = programCode[tokenStart[index] + i];
	tokenValue[(tokenEnd[index] - tokenStart[index]) + 1] = '\0';

	return tokenValue;
}

Interpreter::CodeScope Interpreter::createCodeScope(bool singleLine)
{
	CodeScope code;

	for(; compileCodeIndex < tokenCount; compileCodeIndex++)
	{
		char* tokenValue = getToken(compileCodeIndex);

		if(strcmp(tokenValue, "}") == 0)
		{
			compileCodeIndex++;
			return code;
		}
		else if(strcmp(tokenValue, "object") == 0)
		{
			char* objectName = getToken(compileCodeIndex + 1);

			if(valueInlookupTable(objectName))
			{
				error("object already declared");
				return code;
			}

			addToLookupTable(objectName, 0);

			compileCodeIndex += 2;
			
			addProgramLine(code, {1, lookupTableCount, (unsigned int)&createObject()});

			free(objectName);
		}
		else if(strcmp(tokenValue, "while") == 0){}
		else if(strcmp(tokenValue, "for") == 0){}
		else if(strcmp(tokenValue, "if") == 0){}
		else if(strcmp(tokenValue, "else") == 0){}
		else if(strcmp(tokenValue, "{") == 0)
		{
			compileCodeIndex++;
			addProgramLine(code, {0, (unsigned int)&createCodeScope(false)});
		}
		else if(strcmp(tokenValue, ";") == 0)
		{
			if(singleLine == true)
			{
				compileCodeIndex++;
				return code;
			}
		}
		else if(valueInlookupTable(tokenValue))
		{
			unsigned int lookupTableIndex = addToLookupTable(tokenValue);
			
			if(lookupTableType[lookupTableIndex] == 0) //Object type
			{
				unsigned int objectId = addToLookupTable(tokenValue, 0);
				unsigned int instanceId = addToLookupTableDelete(getToken(compileCodeIndex + 1), 1);
	
				if(strcmp(getToken(compileCodeIndex + 2), "(") == 0) //function
				{
					compileCodeIndex += 3;
	
					unsigned int* inputVariableId = (unsigned int*)malloc(400);
					unsigned int* inputObjectTypeId = (unsigned int*)malloc(400);
					boolean* inputHasValue = (boolean*)malloc(100);
					unsigned int* inputLiteralIndex = (unsigned int*)malloc(400);
	
					unsigned int inputCount = 0;
					unsigned int reservedCount = 100;
	
					while(true)
					{
						char* functionInputToken = getToken(compileCodeIndex);
	
						if(strcmp(getToken(compileCodeIndex), ")") == 0)
						{
							compileCodeIndex++;
							break;
						}
	
						if(reservedCount <= inputCount)
						{
							inputVariableId = (unsigned int*)realloc(inputVariableId, reservedCount * 4 + 400);
							inputObjectTypeId = (unsigned int*)realloc(inputObjectTypeId, reservedCount * 4 + 400);
							inputHasValue = (boolean*)realloc(inputHasValue, reservedCount + 100);
							inputLiteralIndex = (unsigned int*)realloc(inputLiteralIndex, reservedCount * 4 + 400);
						}

						inputCount++;
						
						inputVariableId[inputCount - 1] = addToLookupTableDelete(getToken(compileCodeIndex + 1), 1);
						inputObjectTypeId[inputCount - 1] = addToLookupTable(functionInputToken, 0);

						char* functionToken = getToken(compileCodeIndex + 2);

						if(strcmp(getToken(compileCodeIndex + 2), ",") == 0)
						{
							compileCodeIndex += 3;
						}
						else if(strcmp(getToken(compileCodeIndex + 2), "=") == 0)
						{
							inputHasValue[inputCount - 1] = true;
							inputLiteralIndex[inputCount - 1] = addToLookupTableDelete(getToken(compileCodeIndex + 3), 3);

							if(strcmp(getToken(compileCodeIndex + 4), ",") != 0)
							{
								error("");
								return code;
							}

							compileCodeIndex += 5;
						}
						else
						{
							error("");
							return code;
						}
	
						free(functionInputToken);
					}
	
					inputVariableId = (unsigned int*)realloc(inputVariableId, inputCount * 4);
					inputObjectTypeId = (unsigned int*)realloc(inputObjectTypeId, inputCount * 4);
					inputHasValue = (boolean*)realloc(inputHasValue, inputCount);
					inputLiteralIndex = (unsigned int*)realloc(inputLiteralIndex, inputCount * 4);

					addProgramLine(code, {3, instanceId, objectId, (unsigned int)&createCodeScope(false), inputCount, (unsigned int)inputVariableId, (unsigned int)inputObjectTypeId, (unsigned int)inputHasValue, (unsigned int)inputLiteralIndex});
				}
				else //new instance
				{
					addProgramLine(code, {2, instanceId, objectId});

					if(strcmp(getToken(compileCodeIndex + 2), "=") == 0)
					{
						addProgramLine(code, {4, instanceId, objectId, addToLookupTableDelete(getToken(compileCodeIndex + 2), 2), 1, });
					}
					else if(strcmp(getToken(compileCodeIndex + 2), ";") != 0)
					{
						error("");
						return code;
					}
				}
			}
			else if(lookupTableType[lookupTableIndex] == 1) //Object instance
			{
				
			}
			else if(lookupTableType[lookupTableIndex] == 2) //function
			{
				
			}
		}
		else
		{
			error("error");
			return code;
		}

		free(tokenValue);
	}

	return code;
}

Interpreter::Object Interpreter::createObject()
{
	Object newObject;

	if(strcmp(getToken(compileCodeIndex), "{") != 0) return newObject;

	return newObject;
}

bool Interpreter::valueInlookupTable(char* token)
{
	for(int i = 0; i < lookupTableCount; i++)
	{
		if(strcmp(token, (char*)lookupTable[i]) == 0)
		{
			return true;
		}
	}

	return false;
}

unsigned int Interpreter::addToLookupTable(char* token, byte type)
{
	for(int i = 0; i < lookupTableCount; i++)
	{
		if(strcmp(token, (char*)lookupTable[i]) == 0)
		{
			return i;
		}
	}

	char* tokenCopy = (char*)malloc(strlen(token) + 1);
	for(int i = 0; i <= strlen(token); i++) tokenCopy[i] = token[i];

	lookupTableCount++;
	lookupTable[lookupTableCount - 1] = (unsigned int)tokenCopy;
	lookupTableType[lookupTableCount - 1] = type;

	return lookupTableCount - 1;
}

unsigned int Interpreter::addToLookupTableDelete(char* token, byte type)
{
	for(int i = 0; i < lookupTableCount; i++)
	{
		if(strcmp(token, (char*)lookupTable[i]) == 0)
		{
			free(token);
			return i;
		}
	}

	lookupTableCount++;
	lookupTable[lookupTableCount - 1] = (unsigned int)token;
	lookupTableType[lookupTableCount - 1] = type;

	return lookupTableCount - 1;
}

void Interpreter::addProgramLine(CodeScope code, std::initializer_list<unsigned int> a)
{
	unsigned int* listPointer = (unsigned int*)a.begin();
	code.programLineCount++;
	code.programLinePointer[code.programLineCount - 1] = (unsigned int)malloc(a.size() * 4);
	for(int i = 0; i < a.size(); i++) ((unsigned int*)code.programLinePointer[code.programLineCount - 1])[i] = listPointer[i];
}

void Interpreter::execute(unsigned int milliseconds)
{
	while(true)
	{
		if(scopeCount == 0) return;

		//unsigned int* currentCode = &scopeArray[currentScope].code[0].programTokens[scopeArray[currentScope].codeIndex];
		//unsigned int codeTokensRemaining = scopeArray[currentScope].code[0].programTokensCount - scopeArray[currentScope].codeIndex;

		//
		
		currentScope++;
		if(currentScope >= scopeCount) currentScope = 0;
	}
}

void Interpreter::error(char* errorMessage)
{
	//strlen(errorMessage)
}
