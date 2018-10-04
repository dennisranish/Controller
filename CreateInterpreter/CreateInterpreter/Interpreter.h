#ifndef Interpreter_h
#define Interpreter_h

//#include "Arduino.h" //Add for arduino
#include "vector"//Remove for arduino

class Interpreter
{
	public:
		//enum codeCommands { cmd_createObject, cmd_createFunction, cmd_createInstance, cmd_pushVariable, cmd_call };
		unsigned int* codePointer;
		unsigned int** scope;
		unsigned int* scopeSize;
		class Thread;
		//std::vector<Thread> thread;
		std::vector<unsigned int> threadId;
		
		//void addNativeFunctionVoid(char* name, void (*function)(unsigned int*));
		//void addNativeFunction(char* name, unsigned int* (*function)(unsigned int*));
};

//4 * (4 + 7) + (6 + 7)
//((4, (4, 7, +), *), (6, 7, +), +)

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
