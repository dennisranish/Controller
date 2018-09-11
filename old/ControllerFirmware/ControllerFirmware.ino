#include "Interpreter.h"

/*code =
{
  int coolness = 0;

  int computeCoolness(int a) 6-8
  {
    return a + joystick.left.x;
  }

  while(true) 11-22
  {
    coolness = computeCoolness(coolness);

    if(coolness < 100)
    {
      coolness = coolness + 1;
    }
    else
    {
      coolness = coolness + 2;
    }
  }
}

"int"
"coolness"
"="
"0"
";"
"int"
"computeCoolness"
"("
"int"
"a"
")"
"{"
"reurn"
"a"
"+"
"joystick.left.x"
"}"
"while"
"("
"true"
")"
"{"

code2 =
{
  int coolness;
  coolness = 0;

  jumpToLine 37
  int computeCoolness(int a)
  int 0 = a + joystick.left.x;
  return 0;

  coolness = jumpToWithReturn 34 (coolness);

  boolean 0 = coolness < 100;
  if not(0) jumpToLine 44;
  coolness = coolness + 1;
  jumpToLine 45;
  coolness = coolness + 2;
  
  jumpToLine 38;
}


0=;
55=parent;
1=int;
2=long;
3=float;
4=int function;
5=long function;
6=float function;

{(byte)1,(byte)8,"coolness",(int)0,(byte)4,(byte)15,"computeCoolness",(int)20,"a + joystick.left.x;",(byte)0, (int)pointer}

{(byte)55}



{int,8,"coolness",0,int function,15,"computeCoolness",5,8}

{parent,pointer,returnType non, 10, 22}

{parent,pointer,returnType int,5,8,int,1,"a",0}



{
  .coolness = int 0;
  .computeCoolness = int (int a) {return a + joystick.left.x;};
  .{
    
   }
}*/

char code[] = "  int coolness = 0;\n\n  int computeCoolness(int a)\n  {\n    return a + joystick.left.x;\n  }\n\n  while(true)\n  {\n    coolness = computeCoolness(coolness);\n\n    if(coolness<=100)\n    {\n      coolness=coolness+1;\n    }\n    else\n    {\n      coolness = coolness + 2;\n    }\n  }";

Interpreter program;

void setup()
{
  Serial.begin(115200);

  program.createProgramTokens(&code[0], sizeof(code));
  
  Serial.println(program.numberOfTokens);

  program.numberOfStacks = 1;
  program.stackScopePointer = (unsigned int*)malloc(4);
  program.stackScopePointer[0] = (unsigned int)malloc(50);

  ((byte*)program.stackScopePointer[0])[0] = 0;
  ((byte*)program.stackScopePointer[0])[1] = 0;
  ((byte*)program.stackScopePointer[0])[2] = 0;
  ((byte*)program.stackScopePointer[0])[3] = 0;
  ((byte*)program.stackScopePointer[0])[4] = 0;
  
  /*for(int token = 0; token < program.numberOfTokens; token++)
  {
    for(int i = 0; i < program.tokenLength[token]; i++)
    {
      Serial.print(((char*)program.tokenPointer[token])[i]);
    }

    Serial.println();
  }*/
}

void loop()
{
  Serial.println("loop");
  program.execute(0);
}
