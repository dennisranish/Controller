#ifndef Joystick_h
#define Joystick_h

#include <Arduino.h>
#include <Element.h>

class Joystick : public Element
{
	public:
		Joystick(const String & name);
		Joystick(const String & name, const String & style);

		void setLabels(const String & top, const String & right, const String & bottom, const String & left);
		void setUpdateCallback(void (*newUpdateCallback)(double, double));

		double getX();
		double getY();

	private:
		double xValue = 0;
		double yValue = 0;
		void (*updateCallback)(double, double);

		void connected(uint8_t num);
		void data(uint8_t num, const String & data);
		void disconnected(uint8_t num);
};

#endif