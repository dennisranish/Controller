#ifndef Interpreter_h
#define Interpreter_h

#include "Arduino.h"

class Interpreter
{
	public:
		unsigned int* programTokens;
		unsigned int programTokensCount = 0;
		unsigned int* lookupTable; //Pointers to char arrays (0 terminated)
		byte* lookupTableType;
		unsigned int lookupTableCount = 0;
		
		//unsigned int* stackScopePointer;
		//unsigned int numberOfStacks = 0;
		//unsigned int currentStack = 0;
		
		Interpreter();
		
		void setProgram(char* programCode);
		
	private:
		void error(char errorMessage[]);
};

#endif

//
// int i = 0;
// {}
// i++
// ifnot(i < 10) jumpback 2 //2 (i < 10) 2
// delete i
//

/*
 * tokenize code
 * replace tokens with unsigned ints (every new token get the next number)
 * - reserved names
 */

/*
 * reserved token ints
 * 
 * object
 * '(', ')', '{', '}', '.', ';'
 * if, else if, else
 * while
 * for
 */
