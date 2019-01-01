#ifndef Joystick_h
#define Joystick_h

#include <Arduino.h>
#include <Element.h>
#include <FastString.h>

class Joystick : public Element
{
	public:
		Joystick(char* name);
		Joystick(char* name, char* style);

		void setTitle(char* newTitle);
		void setLabels(char* newTopLabel, char* newRightLabel, char* newBottomLabel, char* newLeftLabel);
		void setUpdateCallback(void (*newUpdateCallback)(double, double));

		double x();
		double y();

	private:
		static char* jsInitCode;
		static char* jsUpdateCode;
		double xValue = 0;
		double yValue = 0;
		char* title = "";
		char *topLabel = "", *rightLabel = "", *bottomLabel = "", *leftLabel = "";
		void (*updateCallback)(double, double) = NULL;

		void connected(uint8_t num);
		void data(uint8_t num, char* data);
		void disconnected(uint8_t num);
};

#endif