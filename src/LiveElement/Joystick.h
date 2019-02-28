#ifndef Joystick_h
#define Joystick_h

#include <Arduino.h>
#include <Element.h>
#include <SingleController.h>

class Joystick : public Element
{
	public:
		Joystick(char * name, char * style = "");

		void setTitle(char * newTitle);
		void setLabels(char * newTopLabel, char * newRightLabel, char * newBottomLabel, char * newLeftLabel);
		void setUpdateCallback(void (*newUpdateCallback)(double, double));

		double x();
		double y();

	private:
		static char * jsCode;
		char * style = "";
		double xValue = 0;
		double yValue = 0;
		char * title = "";
		char * topLabel = "", * rightLabel = "", * bottomLabel = "", * leftLabel = "";
		void (*updateCallback)(double, double) = NULL;

		void connectedEvent(uint8_t num);
		void dataEvent(uint8_t num, char * data);
		void disconnectedEvent(uint8_t num);
};

#endif