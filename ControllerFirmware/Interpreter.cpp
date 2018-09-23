#include "Interpreter.h"

void Interpreter::setProgram(char* code)
{
	codePointer = code;
}

void Interpreter::compileProgram()
{
	//TODO parse code

	//TODO add token zero to add array

	while(/*TODO check if add array size is > 0*/)
	{
		//TODO int compileIndex = add array.addScopeTokenStart[0];

		//TODO check and handle function (if first token == '(' until ')')

		//TODO if this token != '{' bool singleLine = true;

		while(true)
		{
			//TODO if (singleLine && token == ';') || (token == '}') break;

			//TODO if token == '{' add to add array; then go until found the matching '}' (check against go to far); add jump command and 0, in add array add pointer to zero (to be filled in later)

			//TODO if token == 'object' add next token to lookupTable; add createObject command, id, and pointer returned from createObject function (pass pointer to compileIndex variable, function will leave it after object; function will also add functions to add array)

			//TODO handle if token == if, else, while, for

			//TODO if token (in lookupTable) is an objectType add createInstance command and typeId and id (add id to lookupTable)

			//TODO else call evaluate function (it will recursivly call it's self to add commands to code to run edxprestion using reverse polish notation, also pass pointer to compileIndex variable, function will leave it after expretion)
		}

		//TODO remove first item from add array
	}
}
