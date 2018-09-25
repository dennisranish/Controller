#include "Interpreter.h"

void Interpreter::setProgram(char* code)
{
	codePointer = code;
}

void Interpreter::compileProgram()
{	
	//TODO parse code

	//TODO add token zero to add array

	while(true/*TODO check if add array size is > 0*/)
	{
		//TODO int compileIndex = add array.addScopeTokenStart[0];

		//TODO check and handle function (if first token == '(' until ')')

		//TODO if this token != '{' bool singleLine = true;

		while(true)
		{
			/*
			 * if({)
			 * else if(object) addProgramLine(compileIndex, {createObject, addToLookupTable(compileIndex + 1), (unsigned int)createObjectFromCode(compileIndex)});
			 * else if(if) addIf();
			 * else if(else) addElse();
			 * else if(for) addFor();
			 * else if(while) addWhile();
			 * else if(typeId == object && token + 2 == '(')
			 * else if(typeId == object) addProgramLine(compileIndex, {createInstance, addToLookupTable(compileIndex), addToLookupTable(compileIndex + 1)});
			 * else
			 */
			
			//TODO if (singleLine && token == ';') || (token == '}') break;

			//TODO if token == '{' add to add array; then go until found the matching '}' (check against go to far); add jump command and 0, in add array add pointer to zero (to be filled in later)

			//TODO if token == 'object' add token to lookupTable; save Id in unsigned int objectId;  add createObject command, objectId, (unsigned int)createObjectFromCode(compileIndex) (pass pointer to compileIndex variable, function will leave it after object)

			//TODO handle if token == if, else, while, for

			//TODO if token (in lookupTable) is an objectType
			{
				//TODO if there is '(' (it is a function)
				{
					//TODO create 3 byte function structre
					//TODO add index to add array and add pointer to put code index
					
					//TODO add createFunction command add id (add id to lookupTable) and pointer to function structre
				}
				//TODO else
				{
					//TODO add createInstance command and typeId and id (add id to lookupTable)
				}
			}

			//TODO else call evaluate function (it will recursivly call it's self to add commands to code to run edxprestion using reverse polish notation, also pass pointer to compileIndex variable, function will leave it after expretion)
		}

		//TODO remove first item from add array
	}
}

unsigned int* Interpreter::createObjectFromCode(unsigned int& compileIndex)
{
	unsigned int* newObject = (unsigned int*)malloc(5);
	
	newObject[0] = 0; //TODO replace with id for objects from lookupTable

	//TODO create pointers and count and reserved amount variables

	while(true)
	{
		//TODO if token == '}' compileIndex++; break;

		//TODO if token == 'object' add object to function/object list (add id (add id to lookupTable) and return value from createObjectFromCode())

		//TODO if token (in lookupTable) is an objectType
		{
			//TODO if there is '(' (it is a function)
			{
				//TODO create 3 byte function structre
				//TODO add index to add array and add pointer to put code index
				
				//TODO add function to function/object list (add id (add id to lookupTable) and pointer to function structre)
			}
			//TODO else
			{
				//TODO add to variable list (add typeId and id (add id to lookupTable))
			}
		}
	}
	
	return newObject;
}
