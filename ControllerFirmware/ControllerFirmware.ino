#include "Interpreter.h"

char* code = "  int coolness = 0;\n\n  int computeCoolness(int a)\n  {\n    return a + joystick.left.x;\n  }\n\n  while(true)\n  {\n    coolness = computeCoolness(coolness);\n\n    if(coolness<=100)\n    {\n      coolness=coolness+1;\n    }\n    else\n    {\n      coolness = coolness + 2;\n    }\n  }\0";

Interpreter program;

void setup()
{
	Serial.begin(115200);
	program.setProgram(code);
}

void loop()
{
}
