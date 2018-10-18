#include "Interpreter.h"

void Interpreter::Thread::executeLine(Interpreter parent)
{
	unsigned int &codeIndex = ((unsigned int*)scope)[3];
	unsigned int *code = (unsigned int*)parent.codePointer[codeIndex];

	if (code[0] == cmd_newScope)
	{
		//code[1] = sizeInBytes;
		//code[2] = parameterCount;
		//code[5] = codeLutIndex;
		codeIndex += 4;
	}
	else if (code[0] == cmd_callNative)
	{
		//code[1] = nativeFunctionIndex
		codeIndex += 2;
	}
	else if (code[0] == cmd_push)
	{
		//code[1] = scopesBack
		//code[2] = indexFromThere
		codeIndex += 3;
	}
	else if (code[0] == cmd_pushLiteral)
	{
		//code[1] = pointer
		codeIndex += 2;
	}
	else if (code[0] == cmd_copyParameter)
	{
		//code[1] = sizeInBytes
		//code[2] = positionToCopy
		//code[3] = defaultValuePointer
		codeIndex += 4;
	}
	else if (code[0] == cmd_jump)
	{
		//code[1] = jumpAmount(signed)
		codeIndex += 2;
	}
	else if (code[0] == cmd_checkJump)
	{
		//code[1] = jumpAmount(signed)
		codeIndex += 2;
	}
	else if (code[0] == cmd_endScope)
	{
		//code[1] = amount
		//code[2] = jumpAmount
		codeIndex += 3;
	}
	else if (code[0] == cmd_clearWS)
	{
		codeIndex += 1;
	}
	else
	{
		//TODO: error("Fatal error, program is corrupt")
	}
}