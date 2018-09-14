#include "Interpreter.h"

Interpreter::Interpreter()
{
}

void Interpreter::setProgram(char* programCode)
{
	unsigned int* tokenStart = (unsigned int*)malloc(400);
	unsigned int* tokenEnd = (unsigned int*)malloc(400);

	unsigned int tokenCount = 0;
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

	programTokens = (unsigned int*)malloc(tokenCount * 4);
	programTokensCount = tokenCount;

	lookupTable = (unsigned int*)malloc(400);

	unsigned int reservedLookupTableCount = 100;

	for(int a = 0; a < tokenCount; a++)
	{
		char* tokenValue = (char*)malloc((tokenEnd[a] - tokenStart[a]) + 2);

		for(int i = 0; i <= tokenEnd[a] - tokenStart[a]; i++) tokenValue[i] = programCode[tokenStart[a] + i];
		tokenValue[(tokenEnd[a] - tokenStart[a]) + 1] = '\0';

		bool isTokenInLookupTable = false;

		for(int i = 0; i < lookupTableCount; i++)
		{
			if(strcmp(tokenValue, (char*)lookupTable[i]) == 0)
			{
				programTokens[a] = i;
				isTokenInLookupTable = true;
				free(tokenValue);
				break;
			}
		}

		if(!isTokenInLookupTable)
		{
			if(lookupTableCount >= reservedLookupTableCount)
			{
				lookupTable = (unsigned int*)realloc(lookupTable, reservedLookupTableCount * 4 + 400);

				reservedLookupTableCount += 100;
			}
			
			lookupTableCount++;
			lookupTable[lookupTableCount - 1] = (unsigned int)tokenValue;
			programTokens[a] = lookupTableCount - 1;
		}
	}

	lookupTable = (unsigned int*)realloc(lookupTable, lookupTableCount * 4);

	/*for(int a = 0; a < programTokensCount; a++)
	{
		Serial.println((char*)lookupTable[programTokens[a]]);
	}*/

	/*
	 * turn program in to a scoped structure
	 * 
	 * ex:
	 * 
	 * int a = 0;
	 * while(a < 7)
	 * {
	 * a++;
	 * }
	 * 
	 * .0{[int], [a], [=], [0], [;], [while], [(], [a], [<], [7], [)], pointer to 1}
	 * .1{[a], [++], [;]}
	 * 
	 * something along those lines...
	 * 
	 * unsigned int* code = 
	 */
}

void Interpreter::error(char errorMessage[])
{
}
