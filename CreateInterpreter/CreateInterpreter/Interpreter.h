#ifndef Interpreter_h
#define Interpreter_h

//#include "Arduino.h" //Add for arduino
#include "vector"//Remove for arduino

class Interpreter
{
	public:
		unsigned int* codePointer;
		void(**function)(uintptr_t parentScope, uintptr_t previousScope, unsigned int parameterCount, uintptr_t thisPointer);
		unsigned int* codePointerLut;
		unsigned int* typeSizeLut;
		class Thread
		{
			public:
				enum codeCommands { cmd_newScope, cmd_callNative, cmd_push, cmd_pushLiteral, cmd_copyParameter, cmd_jump, cmd_checkJump, cmd_endScope, cmd_returnValue, cmd_clearWS };
				/*
				newScope sizeInBytes(unsgined)(from scope list create by compiler) parameterCount(unsigned) codeLutIndex(unsigned) //codeIndex is already set and variables are initialized; previous scope needs to be set
				callNative nativeFunctionIndex(unsigned) //function recives pointerToScope
				push scopesBack(unsigned) indexFromThere(unsigned)
				pushLiteral id(unsigned)
				copyParameter sizeInBytes(unsigned) positionToCopy(unsigned) defaultValuePointer(unsigned)
				jump jumpAmount(signed) //adds jumpAmount to codeIndex
				checkJump jumpAmount(signed) //adds jumpAmount to codeIndex if top value in workingSrack is false (removes top element from workingStack) otherwise adds 2 (to get to the next line)
				endScope amount(unsigned) jumpAmount(unsigned) //ends that many scopes
				returnValue amount(unsigned) //ends that many scopes, copies value and adds to workingstack
				clearWS //clears working stack
				*/

				uintptr_t scope; //previous, parent, workingStackPointer, workingStackType, workingStackDetails, codeIndex, parametersLeft, data...
				const static unsigned int ScopeMinSize = 7 * 4;

				//workingStackType: isPointer

				void executeLine(Interpreter parent);
		};
		std::vector<Thread> thread;
		std::vector<unsigned int> threadId;
		
		//void addNativeFunctionVoid(char* name, void (*function)(unsigned int*));
		//void addNativeFunction(char* name, unsigned int* (*function)(unsigned int*));
		//void addNativeFunction(char* name, void(*function)(uintptr_t parentScope, uintptr_t previousScope, unsigned int parameterCount, uintptr_t thisPointer));
};

//4 * (4 + 7) + (6 + 7)
//((4, (4, 7, +), *), (6, 7, +), +)

/*To get to any variable go up through the scope some number of times (one number) then the index*/
/*Scopes are always a set size*/
/*public, private, object variable offset, etc. are all done by the compiler. The compiler HARD CODES in locations for variables*/

/*
 * commands have variable amount of parameters
 * 
 * addScopeTokenStart
 * putIndexInPointer 1 or 0
 * pointerToPutIndex
 */

 /*
  * createObject id pointerToPreCreatedObjectType
  * createInstance typeId, id;
  * createFunction id, pointerToPreCreatedFunction
  * 
  * createParameterInstance typeId, id //creates instance and if paramters are left assigned value of parameter
  */

  /*
   * objectType: (length 5 bytes)
   * lookupTableId(object)
   * variable count
   * function/objectType count
   * variablesPointer = [variableTypeId, variableId]
   * functions/objectsPointer = [functions/objectsId, functions/objectsPointer]
   * 
   * function: (length 3 bytes)
   * lookupTableId(function)
   * returnTypeId
   * codeIndex (firstLine)
   * 
   * instance:
   * lookupTableId (objectType)
   * pointerToObjectType
   * [variableId, variablePointer]
   */

#endif
