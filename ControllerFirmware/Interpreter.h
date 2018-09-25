#ifndef Interpreter_h
#define Interpreter_h

#include "Arduino.h"
#include "Compiler.h"
#include "Thread.h"

class Interpreter
{
	public:
		enum CodeCommand {createObject, createInstance, createFunction};
		char* codePointer;
		std::vector<Thread> threadArray;
		Compiler compiler;

		void setProgram(char* code);
		void compileProgram();
		unsigned int* createObjectFromCode(unsigned int& compileIndex);

		void addObject();
		void addFunction();
		void addInstance();
		void addIf();
		void addElseIf();
		void addElse();
		void addFor();
		void addWhile();
		
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
