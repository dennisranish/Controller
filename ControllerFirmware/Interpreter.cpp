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

	unsigned int characterType = 0;

	for(int i = 0; true; i++)
	{
		if(programCode[i] == '\0')
		{
			tokenEnd[tokenCount - 1] = i - 1;
			break;
		}
		
		unsigned int newCharacterType = 0;
		char a = programCode[i];

		if(a == ' ' || a == '\n' || a == '\t') newCharacterType = 0;
		else if(a == '{' || a == '}' || a == '(' || a == ')' || a == '[' || a == ']' || a == '.' || a == ';') newCharacterType = 1;
		else if((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || (a >= '0' && a <= '9') || a == '_') newCharacterType = 2;
		else newCharacterType = 3;

		if(newCharacterType != characterType)
		{
			if(tokenCount > 0 && characterType != 0) tokenEnd[tokenCount - 1] = i - 1;
			if(newCharacterType != 0)
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
	
	//if = ifnot() *jumpPos*, {jump *jumpPos*}
	//else if = ifnot() *jumpPos*, {jump *jumpPos*}
	//else = {}
	//while = ifnot() *jumpPos*, {}, jump *jumpPos back*
	//for = declare statment, ifnot() *jumpPos*, {}, jump *jumpPos back*
	

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
}

void Interpreter::error(char errorMessage[])
{
}
