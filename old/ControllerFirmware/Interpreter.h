#ifndef Interpreter_h
#define Interpreter_h

#include "Arduino.h"

class Interpreter
{
  public:
    unsigned int* tokenPointer;
    unsigned short* tokenLength;
    unsigned int numberOfTokens = 0;
    
    unsigned int* stackScopePointer;
    unsigned int numberOfStacks = 0;
    unsigned int currentStack = 0;

    Interpreter();
    
    void createProgramTokens(char* programCode, int programCodeLength);
    void newToken();
    void addToToken(char newChar);

    void setupProgram();

    void execute(int milliseconds);

  private:
    void error(char errorMessage[]);

    bool checkForToken(unsigned int tokenIndex, ...);
};

#endif
