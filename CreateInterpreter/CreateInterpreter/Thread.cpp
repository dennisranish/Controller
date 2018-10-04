#include "Thread.h"

void Interpreter::Thread::executeLine(Interpreter parent)
{
	unsigned int &codeIndex = ((unsigned int*)scope)[3];
	unsigned int *code = (unsigned int*)parent.codePointer[codeIndex];

	if (code[0] == cmd_newScope)
	{
		unsigned int newScopeSize = parent.scopeSize[code[1]];
		uintptr_t newScope = (uintptr_t)malloc(newScopeSize);
		memcpy((void*)newScope, (void*)parent.scope[code[1]], newScopeSize);
		scope = newScope;
		codeIndex += 2;
	}
	else if (code[0] == cmd_push)
	{
		std::vector<unsigned int> workingStack = (std::vector<unsigned int>)((unsigned int*)scope)[2];
		workingStack.push_back(code[1]);
	}
	else if (code[0] == cmd_callNative)
	{

	}
}