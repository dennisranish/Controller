#include "Interpreter.h"

Interpreter::Interpreter()
{
}

void Interpreter::setProgram(char* programCode)
{
	unsigned int* tokenPointer;
	unsigned int tokenCount = 0;

	unsigned int tokenLength = 0;

	/*
	0-white space: not add into tokens: ' ', '\n', (tab)
	1-special characters: each is in it's own token: {}().;
	2-alphabatic characters: on type change create new token: a-z, A-Z, 0-9, _
	3-all other symbols: on type change create new token
	*/

	unsigned int characterType = 0;

	for(int i = 0; programCode[i] != '\0'; i++)
	{
		Serial.println();
		Serial.print(i);
		Serial.print(" : ");
		Serial.print(programCode[i]);
		Serial.print(" : ");
		Serial.print("1,");
		unsigned int newCharacterType = 0;
		char a = programCode[i];

		if(a == ' ' || a == '\n' || a == '\t') newCharacterType = 0;
		else if(a == '{' || a == '}' || a == '(' || a == ')' || a == '.' || a == ';') newCharacterType = 1;
		else if((a >= 65 && a <= 90) || (a >= 97 && a <= 122) || (a >= 48 && a <= 57) || a == 95) newCharacterType = 2;
		else newCharacterType = 3;

		Serial.print("2,");
		
		if(newCharacterType == 0)
		{
			characterType = newCharacterType;
			continue;
		}

		Serial.print("3,");
		
		if(newCharacterType != characterType || newCharacterType == 1)
		{
			Serial.print("4,");
			if(tokenCount > 0)
			{
				Serial.print("5,");
				tokenPointer[tokenCount - 1] = (unsigned int)realloc((unsigned int*)tokenPointer[tokenCount - 1], tokenLength + 1);
				((byte*)tokenPointer[tokenCount - 1])[tokenLength] = '\0';
				Serial.print("6,");
			}
			Serial.print("7,");
			if(tokenCount == 0) tokenPointer = (unsigned int*)malloc(tokenCount + 1);
			else tokenPointer = (unsigned int*)realloc(tokenPointer, tokenCount + 1);
			tokenCount++;
			tokenLength = 0;
			characterType = newCharacterType;
			Serial.print("8,");
		}

		Serial.print("9,t: ");
		Serial.print(tokenLength);
		if(tokenLength == 0) tokenPointer[tokenCount - 1] = (unsigned int)malloc(1);
		else tokenPointer[tokenCount - 1] = (unsigned int)realloc((unsigned int*)tokenPointer[tokenCount - 1], tokenLength + 1);Serial.print("10,");
		((byte*)tokenPointer[tokenCount - 1])[tokenLength] = programCode[i];Serial.print("11,");
		tokenLength++;
		Serial.print("12,");
	}
	tokenPointer[tokenCount - 1] = (unsigned int)realloc((unsigned int*)tokenPointer[tokenCount - 1], ++tokenLength);
	((byte*)tokenPointer[tokenCount - 1])[tokenLength - 1] = '\0';

	/*int i = 0;
	while(programCode[i] != '\0')
	{
		if(programCode[i] == ' ' || programCode[i] == '\n' || programCode[i] == '\t') characterType = 0;
		else if(programCode[i] == '{' || programCode[i] == '}' || programCode[i] == '(' || programCode[i] == ')' || programCode[i] == '.' || programCode[i] == ';')
		{
			if(tokenCount > 0)
			{
				tokenPointer[tokenCount - 1] = (unsigned int)realloc((unsigned int*)tokenPointer[tokenCount - 1], tokenLength * 4 + 4);
				((byte*)tokenPointer[tokenCount - 1])[tokenLength] = '\0';
			}
			tokenPointer = (unsigned int*)realloc(tokenPointer, tokenCount * 4 + 4);
			tokenCount++;
			tokenLength = 0;

			characterType = 1;
			if(tokenLength == 0) tokenPointer[tokenCount - 1] = (unsigned int)malloc(4);
			else tokenPointer[tokenCount - 1] = (unsigned int)realloc((unsigned int*)tokenPointer[tokenCount - 1], tokenLength * 4 + 4);
			((byte*)tokenPointer[tokenCount - 1])[tokenLength] = programCode[i];
			tokenLength++;
		}
		else if((programCode[i] >= 65 && programCode[i] <= 90) || (programCode[i] >= 97 && programCode[i] <= 122) || (programCode[i] >= 48 && programCode[i] <= 57) || programCode[i] == 95)
		{
			if(characterType != 2)
			{
				if(tokenCount > 0)
				{
					tokenPointer[tokenCount - 1] = (unsigned int)realloc((unsigned int*)tokenPointer[tokenCount - 1], tokenLength * 4 + 4);
					((byte*)tokenPointer[tokenCount - 1])[tokenLength] = '\0';
				}
				tokenPointer = (unsigned int*)realloc(tokenPointer, tokenCount * 4 + 4);
				tokenCount++;
				tokenLength = 0;
			}

			characterType = 2;
			if(tokenLength == 0) tokenPointer[tokenCount - 1] = (unsigned int)malloc(4);
			else tokenPointer[tokenCount - 1] = (unsigned int)realloc((unsigned int*)tokenPointer[tokenCount - 1], tokenLength * 4 + 4);
			((byte*)tokenPointer[tokenCount - 1])[tokenLength] = programCode[i];
			tokenLength++;
		}
		else
		{
			if(characterType != 3)
			{
				if(tokenCount > 0)
				{
					tokenPointer[tokenCount - 1] = (unsigned int)realloc((unsigned int*)tokenPointer[tokenCount - 1], tokenLength * 4 + 4);
					((byte*)tokenPointer[tokenCount - 1])[tokenLength] = '\0';
				}
				tokenPointer = (unsigned int*)realloc(tokenPointer, tokenCount * 4 + 4);
				tokenCount++;
				tokenLength = 0;
			}

			characterType = 3;
			if(tokenLength == 0) tokenPointer[tokenCount - 1] = (unsigned int)malloc(4);
			else tokenPointer[tokenCount - 1] = (unsigned int)realloc((unsigned int*)tokenPointer[tokenCount - 1], tokenLength * 4 + 4);
			((byte*)tokenPointer[tokenCount - 1])[tokenLength] = programCode[i];
			tokenLength++;
		}

		i++;
	}
	tokenPointer[tokenCount - 1] = (unsigned int)realloc((unsigned int*)tokenPointer[tokenCount - 1], tokenLength * 4 + 4);
	((byte*)tokenPointer[tokenCount - 1])[tokenLength] = '\0';*/

	for(int a = 0; a < tokenCount; a++) Serial.println((char*)tokenPointer[a]);
}

void Interpreter::error(char errorMessage[])
{
}
