#ifndef Compiler_h
#define Compiler_h

//#include "Arduino.h" //Add for arduino
#include "vector"//Remove for arduino
#include <stdarg.h>
#include "Interpreter.h"

class Compiler
{
	public:
		void(**nativeFunction)(uintptr_t scope);
		char** nativeFunctionName;
		unsigned int nativeFunctionCount = 0;

		std::vector<char*> lut;

		std::vector<uintptr_t> literalPointer;
		std::vector<unsigned int> literalTypeId;

		enum TreeStructureTypes { tst_scope, tst_scopeForLoop, tst_scopeWhileLoop, tst_scopeFunction, tst_objectType, tst_name };
		enum ActionType { at_normal, at_unexpectedToken, at_closed };

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
			std::vector<unsigned int> variableType;
			unsigned int returnTypeId = 0;
			bool functionInitilized = false;
			bool hasReturn = false;

			std::vector<unsigned int> objectId;
			std::vector<TreeStructure*> object;

			std::vector<unsigned int> nameId;
			std::vector<TreeStructure*> name;

			std::vector<unsigned int> instanceId;
			std::vector<TreeStructure*> nameHost;
			std::vector<TreeStructure*> instanceType;

			std::vector<unsigned int> functionId;
			std::vector<TreeStructure*> function;

			std::vector<unsigned int> code;

			void addToCode(std::initializer_list<unsigned int> addArray) { code.insert(code.end(), addArray.begin(), addArray.end()); }

			unsigned int getCodeStart(std::vector<unsigned int>* codePointerLutP, std::vector<TreeStructure*>* codeTreeStructP) { if (codeLutIndex == UINT32_MAX) { codePointerLutP->push_back(0); codeTreeStructP->push_back(this); codeLutIndex = codePointerLutP->size() - 1; } return codeLutIndex; }
		};

		std::vector<TreeStructure*> allTreeStructures;

		std::vector<TreeStructure*> codeTreeStruct;//when combining code use only this and first treeStructreCode
		std::vector<unsigned int> codePointerLut;//pointers to code, set after tree is generated

		std::vector<TreeStructure*> typeIdLut;
		//0: void, size = 0
		//1: bool, size = 1
		//2: int, size = 4
		//3: double, size = 8

		enum ExprStructTypes { es_literal, es_variable, es_stored , es_operator, es_open, es_close, es_function, es_seperator };

		struct ExprStruct
		{
			ExprStructTypes type;

			union
			{
				unsigned int literalId;
				unsigned int scopesBack;
				unsigned int lutId;
				unsigned int param1;
			};

			union
			{
				unsigned int indexFromThere;
				unsigned int operatorType;
				unsigned int param2;
			};

			union
			{
				TreeStructure* instanceType;
				TreeStructure* function;
				TreeStructure* param3;
			};

			TreeStructure* afterType;

			std::vector<unsigned int> unaryOperators;//lutId, or typeIdLut
			std::vector<bool> unaryIsCast;

			unsigned int funcScopesBack;

			//Literal: literalId                  , TreestructrePointer(type)
			//Variable: scopesBack, indexFromThere, TreestructrePointer(type)
			//Stored:                             , TreestructrePointer(type)
			//Operator: lutId, type{0:normal, 1:cast}, if cast TreeStructre(type)
			//Open:
			//Close:
			//Function:                           , TreestructrePointer
			//Seperator:
		};

		struct TreeStructureReturn { TreeStructure* ts; ActionType at; };
		struct ExprStructReturn { unsigned int returnTypeId; ActionType at; };

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
		bool compileCode(Interpreter *interpreter);

		TreeStructureReturn create(TreeStructure *parent, TreeStructureTypes tsType, bool hasParent, bool singleLine, bool allowInstances, bool allowRunningCode, bool writeCodeToRoot, unsigned int startingSize, bool takeParameters, bool doLoop, unsigned int lutId = UINT32_MAX, unsigned int returnType = 0, bool addAsObject = false);
		ExprStructReturn evaluateExpretion(TreeStructure *thisTS, const char* end, unsigned int preferedType);
		TreeStructureReturn evalExpr(TreeStructure *thisTS, std::vector<ExprStruct> expr);

		bool checkForToken(unsigned int index, const char* token);
		bool isTokenObjectType(unsigned int index, TreeStructure *thisTS); TreeStructure *lastToken; unsigned int lastTokenLength;
		bool isTokenInstance(unsigned int index, TreeStructure *thisTS); unsigned int lastIndex; unsigned int scopesBack; unsigned int indexFromThere;
		bool isTokenFunction(unsigned int index, TreeStructure *thisTS, bool includeOperators); TreeStructure* lastParent;
		char* getTokenFullName(unsigned int index, TreeStructure *thisTS);
		void addNativeFunction(const char* name, void(*function)(uintptr_t scope));
		unsigned int lutGetId(unsigned int index);
		unsigned int typeLutGetId(TreeStructure* type);
		unsigned int getLiteralId(char* data, unsigned int typeId);
		

		std::vector<std::vector<char*>> standardRule;
		std::vector<char> standardRuleMaxTokenLength;

		std::vector<char*> wrapRuleStart;
		std::vector<char*> wrapRuleEnd;
		std::vector<bool> wrapRuleExclude;
		std::vector<std::vector<char*>> wrapRuleExceptions;
		std::vector<char> wrapRuleMaxTokenLength;

		std::vector<unsigned int> tokenStart;
		std::vector<unsigned int> tokenEnd;
		std::vector<unsigned int> tokenType;

		unsigned int tokenIndex;

		char* rawCode;
};

#endif
