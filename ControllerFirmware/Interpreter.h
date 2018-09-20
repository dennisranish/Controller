#ifndef Interpreter_h
#define Interpreter_h

#include "Arduino.h"

class Interpreter
{
	public:
		class CodeScope
		{
			public:
				unsigned int* programLinePointer;
				unsigned int programLineCount = 0;
		};

		class Object
		{
			public:
				unsigned int variableCount = 0;
				unsigned int functionCount = 0;
				
				unsigned int* variableTypeId;
				unsigned int* variableId;
				unsigned int* variableValuePointer;

				unsigned int* functionId;
				unsigned int* functionInputVariableId;
				unsigned int* functionInputObjectTypeId;
				boolean* functionInputHasValue;
				unsigned int* functionInputLiteralIndex;
				unsigned int* returnTypePointer;
				CodeScope code;
			
				/*unsigned int variableCount = 0;
				unsigned int objectCount = 0;
				unsigned int functionCount = 0;
	
				unsigned int* variableId;
				unsigned int* objectTypePointer;
				unsigned int* variablePointer;
	
				unsigned int* objectId;
				unsigned int* objectPointer;
	
				unsigned int* functionId;
				unsigned int* functionInputVariableId;
				unsigned int* functionInputObjectTypeId;
				boolean* functionInputHasValue;
				unsigned int* functionInputLiteralIndex;
				unsigned int* returnTypePointer;
				CodeScope code;*/
		};

		class Scope
		{
			public:
				bool hasParent = false;
				Scope* parent;
				Scope* previousScope;
				Object scopeObject;
				unsigned int returnTypePointer;
				CodeScope* code;
				unsigned int codeIndex;
				bool completeIfCycle = true;
		};

		char* programCode;
		unsigned int* tokenStart;
		unsigned int* tokenEnd;
		unsigned int tokenCount = 0;

		unsigned int compileCodeIndex;
	
		CodeScope topCodeScope;
		unsigned int* lookupTable;
		byte* lookupTableType;
		unsigned int lookupTableCount = 0;
		unsigned int lookupTableReserved = 0;
		
		Scope* scopeArray;
		unsigned int scopeCount = 0;
		unsigned int currentScope = 0;
		
		Interpreter();
		
		void setProgram(char* programCode);
		void parseCode(char* code);
		char* getToken(unsigned int index);
		CodeScope createCodeScope(bool singleLine);
		Object createObject();
		CodeScope appendCodeFromExpression(CodeScope code);
		bool valueInlookupTable(char* token);
		byte getlookupTableType(char* token);
		unsigned int addToLookupTable(char* token, byte type);
		unsigned int addToLookupTableDelete(char* token, byte type);
		void addProgramLine(CodeScope code, std::initializer_list<unsigned int> a);

		void execute(unsigned int milliseconds);
		
	private:
		void error(char* errorMessage);
};

#endif
