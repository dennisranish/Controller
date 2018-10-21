#include "Interpreter.h"
#include <ctime>

bool Interpreter::Thread::executeLine(Interpreter *parent)
{
	unsigned int &codeIndex = ((unsigned int*)scope)[5];
	unsigned int *code = &parent->codePointer[codeIndex];

	if (code[0] == cmd_newScope)
	{
		uintptr_t* newScope = (uintptr_t*)malloc(code[1]);
		for (int i = 0; i < code[1]; i++) newScope[i] = 0;
		newScope[0] = scope;
		if(code[4] == 0) newScope[1] = 0;
		else
		{
			uintptr_t parentScope = scope;
			for (int i = 1; i < code[1]; i++) parentScope = (&parentScope)[1];
			newScope[1] = parentScope;
		}
		newScope[2] = (unsigned int)(new std::vector<unsigned int>());
		newScope[3] = (unsigned int)(new std::vector<unsigned int>());
		newScope[4] = (unsigned int)(new std::vector<char>());
		newScope[5] = parent->codePointerLut[code[5]];
		newScope[6] = code[2];

		scope = (unsigned int)newScope;
		//code[1] = sizeInBytes;
		//code[2] = parameterCount;
		//code[3] = codeLutIndex;
		//code[4] = parentScopesBack;
		codeIndex += 5;
	}
	else if (code[0] == cmd_callNative)
	{
		parent->function[code[1]](scope);
		//code[1] = nativeFunctionIndex
		codeIndex += 2;
	}
	else if (code[0] == cmd_push)
	{
		uintptr_t pushScope = scope;
		for (int i = 0; i < code[1]; i++) pushScope = (&pushScope)[1];
		((std::vector<unsigned int>)((&scope)[2])).push_back((unsigned int)(&(&pushScope)[code[2]]));
		((std::vector<unsigned int>)((&scope)[3])).push_back(code[3]);
		((std::vector<unsigned int>)((&scope)[4])).push_back(0);
		//code[1] = scopesBack
		//code[2] = indexFromThere
		//code[3] = typeId
		codeIndex += 4;
	}
	else if (code[0] == cmd_pushLiteral)
	{
		((std::vector<unsigned int>)((&scope)[2])).push_back(code[1]);
		((std::vector<unsigned int>)((&scope)[3])).push_back(code[2]);
		((std::vector<unsigned int>)((&scope)[4])).push_back(1);
		//code[1] = pointer
		//code[2] = typeId
		codeIndex += 3;
	}
	else if (code[0] == cmd_copyParameter)
	{
		if ((&scope)[6] > 0)
		{
			uintptr_t parentScope = (&scope)[1];
			memcpy(&(&scope)[code[2]], &((std::vector<unsigned int>)(&parentScope)[2]).back(), code[1]);
			((std::vector<unsigned int>)((&parentScope)[2])).pop_back();
			((std::vector<unsigned int>)((&parentScope)[3])).pop_back();
			((std::vector<unsigned int>)((&parentScope)[4])).pop_back();
			(&scope)[6]--;
		}
		else
		{
		}
		//code[1] = sizeInBytes
		//code[2] = positionToCopy
		//code[3] = defaultValuePointer
		codeIndex += 4;
	}
	else if (code[0] == cmd_jump)
	{
		codeIndex += (signed int)code[1];
		//code[1] = jumpAmount(signed)
		//codeIndex += 2;
	}
	else if (code[0] == cmd_checkJump)
	{
		if(((std::vector<unsigned int>)((&scope)[2])).back() == false) codeIndex += (signed int)code[1];
		else codeIndex += 2;
		((std::vector<unsigned int>)((&scope)[2])).pop_back();
		((std::vector<unsigned int>)((&scope)[3])).pop_back();
		((std::vector<unsigned int>)((&scope)[4])).pop_back();
		//code[1] = jumpAmount(signed)
		//codeIndex += 2;
	}
	else if (code[0] == cmd_endScope)
	{
		uintptr_t previousScope = scope;
		for (int i = 0; i < code[1]; i++)
		{
			if ((&scope)[0] == NULL)
			{
				free(&scope);
				parent->endOfThread = true;
				return true;
			}
			previousScope = scope;
			scope = (&scope)[0];
			free(&scope);
		}
		((unsigned int*)scope)[3] += code[2];
		//code[1] = amount
		//code[2] = jumpAmount
		//codeIndex += 3;
	}
	else if (code[0] == cmd_clearWS)
	{
		((std::vector<unsigned int>)((&scope)[2]));// .clear();
		((std::vector<unsigned int>)((&scope)[3]));// .clear();
		((std::vector<unsigned int>)((&scope)[4]));// .clear();
		codeIndex += 1;
	}
	else
	{
		return false;
	}

	return true;
}

void Interpreter::startThread()
{
	Thread newThread;
	
	uintptr_t* newScope = (uintptr_t*)malloc(globalSize);
	for (int i = 0; i < globalSize; i++) newScope[i] = 0;
	newScope[0] = NULL;
	newScope[1] = 0;
	newScope[2] = (unsigned int)(new std::vector<unsigned int>());
	newScope[3] = (unsigned int)(new std::vector<unsigned int>());
	newScope[4] = (unsigned int)(new std::vector<char>());
	newScope[5] = 0;
	newScope[6] = 0;

	newThread.scope = (unsigned int)newScope;

	thread.push_back(newThread);
}

bool Interpreter::runProgram(unsigned long millisceonds)
{
	clock_t begin = clock();

	while ((double(clock() - begin) / CLOCKS_PER_SEC) * 1000.0 < millisceonds)
	{
		if (thread.size() == 0)
		{
			return false;
		}

		endOfThread = false;
		bool result = thread[currentThread].executeLine(this);
		if (result == false)
		{
			error = true;
			return false;
		}
		if (endOfThread == true)
		{
			thread.pop_back();
		}

		currentThread++;
		if (currentThread >= thread.size()) currentThread = 0;
	}

	return true;
}

void Interpreter::kill()
{
	while(thread.size() > 0)
	{
		uintptr_t previousScope = thread[thread.size() - 1].scope;
		while ((&(thread[thread.size() - 1].scope))[0] != NULL)
		{
			previousScope = thread[thread.size() - 1].scope;
			thread[thread.size() - 1].scope = ((unsigned int*)(thread[thread.size() - 1].scope))[0];
			//free(&previousScope);
		}
		//free(&thread[thread.size() - 1].scope);
		thread.pop_back();
	}
}