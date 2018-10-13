#ifndef Compiler_h
#define Compiler_h

//#include "Arduino.h" //Add for arduino
#include "vector"//Remove for arduino
#include <stdarg.h>
#include "Interpreter.h"

class Compiler
{
	public:
		void(**nativeFunction)(uintptr_t parentScope, uintptr_t previousScope, unsigned int parameterCount, uintptr_t thisPointer);
		char** nativeFunctionName;
		unsigned int nativeFunctionCount = 0;

		std::vector<char*> lut;

		std::vector<uintptr_t> literalPointer;
		std::vector<unsigned int> literalSize;

		std::vector<unsigned int> codePointerLut;//pointers to code, set after tree is generated

		enum TreeStructureTypes { tst_scope, tst_scopeLoop, tst_scopeConditional, tst_objectType, tst_name };
		enum ActionType { at_normal, at_breakLoop, at_continueLoop, at_unexpectedToken };

		struct TreeStructure
		{
			TreeStructureTypes type;
			unsigned int size;
			bool hasParent = false;
			TreeStructure* parent;
			TreeStructure* codeRoot;
			unsigned int codeLutIndex = UINT32_MAX;
			unsigned int lutId = UINT32_MAX;
			unsigned int minVariables = 0;
			unsigned int maxVariables = 0;

			std::vector<unsigned int> objectId;//unneeded
			std::vector<TreeStructure*> object;

			std::vector<unsigned int> nameId;//unneeded
			std::vector<TreeStructure*> name;

			std::vector<unsigned int> instanceId;
			std::vector<TreeStructure*> instanceType;

			std::vector<unsigned int> functionId;//unneeded
			std::vector<TreeStructure*> function;

			std::vector<unsigned int> code;

			void addToCode(std::initializer_list<unsigned int> addArray) { code.insert(code.end(), addArray.begin(), addArray.end()); }

			unsigned int getCodeStart(std::vector<unsigned int>* codePointerLut) { if (codeLutIndex == UINT32_MAX) { codePointerLut->push_back(0); codeLutIndex = codePointerLut->size() - 1; } return codeLutIndex; }
		};

		std::vector<TreeStructure*> allTreeStructures;

		struct TreeStructreReturn { TreeStructure* ts; ActionType at; };

		//Parser functions
		void addStandardRule(std::initializer_list<const char*> rule, char maxTokenLength = 0);
		void addWrapRule(const char* start, const char* end, bool exclude, std::initializer_list<const char*> exceptions = { "" }, char maxTokenLength = 0);

		bool compareStringTillEnd(const char* stringA, const char* stringB);
		unsigned int findStringInVector(const char* stringA, std::vector<char*> stringVector);
		bool checkCharInRange(const char charA, const char* stringRange);
		unsigned int findStringInRule(const char* stringA);
		void parseCode(const char* code);

		//Token array getters
		unsigned int getSize(){return tokenStart.size();}
		char* getToken(unsigned int index)
		{	
			char* tokenValue = (char*)malloc((tokenEnd[index] - tokenStart[index]) + 2);
		
			for(unsigned int i = 0; i <= (tokenEnd[index] - tokenStart[index]); i++) tokenValue[i] = rawCode[tokenStart[index] + i];
			tokenValue[(tokenEnd[index] - tokenStart[index]) + 1] = '\0';
		
			return tokenValue;
		}

		//Program compile functions
		void compileCode();

		TreeStructreReturn create(TreeStructure *parent, TreeStructureTypes tsType, bool hasParent, bool singleLine, bool allowInstances, bool allowRunningCode, bool writeCodeToRoot, unsigned int startingSize, bool takeParameters, bool doLoop, unsigned int lutId);

		bool checkForToken(unsigned int index, const char* token);
		bool isTokenObjectType(unsigned int index, TreeStructure *thisTS); TreeStructure *lastToken;
		char* getTokenFullName(unsigned int index, TreeStructure *thisTS);
		void addNativeFunction(const char* name, void(*function)(uintptr_t parentScope, uintptr_t previousScope, unsigned int parameterCount, uintptr_t thisPointer));
		unsigned int lutGetId(unsigned int index);
		
	private:
		std::vector<std::vector<char*>> standardRule;
		std::vector<char> standardRuleMaxTokenLength;

		std::vector<char*> wrapRuleStart;
		std::vector<char*> wrapRuleEnd;
		std::vector<bool> wrapRuleExclude;
		std::vector<std::vector<char*>> wrapRuleExceptions;
		std::vector<char> wrapRuleMaxTokenLength;

		std::vector<unsigned int> tokenStart;
		std::vector<unsigned int> tokenEnd;

		unsigned int tokenIndex;

		char* rawCode;
};

#endif
