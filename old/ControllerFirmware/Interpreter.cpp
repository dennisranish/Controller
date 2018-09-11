#include "Interpreter.h"

Interpreter::Interpreter()
{
  
}

void Interpreter::createProgramTokens(char* programCode, int programCodeLength)
{
  char nextChar;
  char newChar;
  char previousChar;

  bool tokenEnded = true;
  bool tokenIsString = false;
  bool isSingleLineComment = false;
  bool isMultiLineComment = false;
  
  for(int programCodeIndex = 0; programCodeIndex < programCodeLength; programCodeIndex++)
  {
    newChar = programCode[programCodeIndex];
    if(programCodeIndex + 1 < programCodeLength) nextChar = programCode[programCodeIndex + 1];
    else nextChar = ' ';

    if(isSingleLineComment)
    {
      if(newChar == '\n')
      {
        isSingleLineComment = false;
        tokenEnded = true;
      }
    }
    else if(isMultiLineComment)
    {
      if(previousChar == '*' && newChar == '/')
      {
        isMultiLineComment = false;
        tokenEnded = true;
      }
    }
    else if(tokenIsString)
    {
      if((newChar == '"' || newChar == '\'') && previousChar != '\\')
      {
        tokenIsString = false;
        tokenEnded = true;
      }
      
      addToToken(newChar);
    }
    else if(newChar == '/' && nextChar == '/')
    {
      isSingleLineComment = true;
    }
    else if(newChar == '/' && nextChar == '*')
    {
      isMultiLineComment = true;
    }
    else if(newChar == ' ' || newChar == '\n')
    {
      tokenEnded = true;
    }
    else
    {
      bool isSingleChar = false;
      bool isDoubleChar = false;
      
      const char doubleCharTokens[] = "== >= <= && || ++ -- += -= *= /=";
      const char singleCharTokens[] = "=+-*/(){}[];,<>";
      
      for(int i = 0; i < strlen(doubleCharTokens); i += 3)
      {
        if(newChar == doubleCharTokens[i] && nextChar == doubleCharTokens[i + 1]) 
        {
          isDoubleChar = true;
          break;
        }
      }
      
      if(!isDoubleChar)
      {
        for(int i = 0; i < strlen(singleCharTokens); i++) 
        {
          if(newChar == singleCharTokens[i])
          {
            isSingleChar = true;
            break;
          }
        }
      }

      if(isDoubleChar)
      {
        newToken();
        addToToken(newChar);
        addToToken(nextChar);
        programCodeIndex++;
        tokenEnded = true;
      }
      else if(isSingleChar)
      {
        newToken();
        addToToken(newChar);
        tokenEnded = true;
      }
      else if(newChar == '"' || newChar == '\'')
      {
        tokenIsString = true;
        tokenEnded = false;
        newToken();
        addToToken(newChar);
      }
      else
      {
        if(tokenEnded == true)
        {
          newToken();
          tokenEnded = false;
        }

        addToToken(newChar);
      }
    }

    previousChar = newChar;
  }
}

void Interpreter::newToken()
{
  if(numberOfTokens == 0)
  {
    tokenPointer = (unsigned int*)malloc(4);
    tokenLength = (unsigned short*)malloc(2);
  }
  else
  {
    tokenPointer = (unsigned int*)realloc(tokenPointer, numberOfTokens * 4 + 4);
    tokenLength = (unsigned short*)realloc(tokenLength, numberOfTokens * 2 + 2);
  }

  numberOfTokens++;
  tokenLength[numberOfTokens - 1] = 0;
}

void Interpreter::addToToken(char newChar)
{
  if(tokenLength[numberOfTokens - 1] == 65535) error("Token exceeded max size of 65536 bytes.");
  
  if(tokenLength[numberOfTokens - 1] == 0)
  {
    tokenPointer[numberOfTokens - 1] = (int)malloc(1);
  }
  else
  {
    tokenPointer[numberOfTokens - 1] = (int)realloc((char*)tokenPointer[numberOfTokens - 1], tokenLength[numberOfTokens - 1] + 1);
  }

  tokenLength[numberOfTokens - 1]++;
  ((char*)tokenPointer[numberOfTokens - 1])[tokenLength[numberOfTokens - 1] - 1] = newChar;
}

void Interpreter::setupProgram()
{
  // TODO:
}

void Interpreter::execute(int milliseconds)
{
  // TODO: handle milliseconds (function should return after at most int milliseconds)
  
  while(true)
  {
    if(numberOfStacks == 0) return;

    byte scopeHeader = ((byte*)stackScopePointer[currentStack])[0];
    unsigned int* scopeInts = (unsigned int*)(stackScopePointer[currentStack] + 1);

    byte* gg = (byte*)stackScopePointer[currentStack];

    int* vv = (int*)(gg);

    if(bitRead(scopeHeader, 0) == 0) //functions/classes have not been checked
    {
      // TODO: handle adding function/class

      //if: void, integer, double, byte, string
      //and if: (+ 2 is '(') or (+ 1 is '[' then ']' and + 4 is '(')

      Serial.println("HI!");

      Serial.println(vv[0]);
      Serial.println((int)vv);
      Serial.println((unsigned int)malloc(3));
      
      Serial.println(scopeInts[0]);

      checkForToken(scopeInts[0], "void", "integer", "double", "byte", "string", 0);

      Serial.println("End!");

      /*if(compareChars(5, tokenLength[scopeInts[0]], (char*)tokenPointer[scopeInts[0]], "void", "integer", "double", "byte", "string"))
      {
        Serial.println(scopeInts[0]);
      }
      else if(strcmp((char*)tokenPointer[scopeInts[0]], "class") == 0)
      {
        
      }*/

      scopeInts[0]++;

      // TODO: handle ending on non open curly bracket in same scope (if one opens fallow it to the end)

      // TODO: if it is a closing curly bracket or last token reset index and mark function/classes as: been checked
    }
    else
    { 
      // TODO: handle variable initilazation

      // TODO: handle * = ... (when variable is set to something)

      // TODO: handle just function calls

      // TODO: handle local new scopes (just {}, if, else if, else, while, for)
    }
    
    currentStack++;
    if(currentStack >= numberOfStacks) currentStack = 0;
  }
}

void Interpreter::error(char errorMessage[])
{
  
}

bool Interpreter::checkForToken(unsigned int tokenIndex, ...)
{ 
  va_list arguments;
  va_start(arguments, tokenIndex);

  bool matchingChars = false;

  Serial.println(tokenIndex);
  Serial.println(tokenLength[tokenIndex]);

  char charArray[tokenLength[tokenIndex] + 1];
  char* charArrayArgument = va_arg(arguments, char*);

  Serial.println("hey");

  for(int i = 0; i < tokenLength[tokenIndex]; i++) charArray[i] = ((char*)tokenPointer[tokenIndex])[i];

  Serial.println("what");
  
  charArray[tokenLength[tokenIndex]] = '\0';
  
  Serial.println("good");

  while(charArrayArgument[0] != '\0')
  {
    if(strcmp(charArray, charArrayArgument) == 0)
    {
      matchingChars = true;
      break;
    }

    charArrayArgument = va_arg(arguments, char*);
  }

  va_end(arguments);

  return matchingChars;
}
