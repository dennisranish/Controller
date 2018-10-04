#ifndef Thread_h
#define Thread_h

//#include "Arduino.h" //Add for arduino
#include "vector"//Remove for arduino
#include "Interpreter.h"

class Interpreter::Thread
{
	public:
		enum codeCommands { cmd_newScope, cmd_callNative, cmd_push };
		/*
		newScope scopeIndex(unsgined)(from scope list create by compiler) parameters(unsigned) //codeIndex is already set and variables are initialized; previous scope needs to be set
		callNative relativeParentIndex(unsigned) previousIndex(unsigned) numberOfVariables(unsigned) //function recives (pointerToScope, variableCount)
		push variablePointer(unsigned)
		*/

		uintptr_t scope; //previous, parent, workingStackPointer, codeIndex, data...

		void executeLine(Interpreter parent);

		/*To get to any variable go up through the scope some number of times (one number) then the index*/
		/*Scopes are always a set size*/
		/*public, private, object variable offset, etc. are all done by the compiler. The compiler HARD CODES in locations for variables*/
};

#endif