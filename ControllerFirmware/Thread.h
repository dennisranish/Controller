#ifndef Thread_h
#define Thread_h

#include "Arduino.h"

class Thread
{
	public:
		class Scope
		{
			public:
				Scope* parentScope;
				Scope* previousScope;
				std::vector<unsigned int> variableId;
				std::vector<unsigned int*> variablePointer;

				std::vector<unsigned int> workingStack;
		};
		
		//unsigned int codeIndex;
		//code pointer???

		//function to run one line
};

#endif
