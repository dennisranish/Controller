#ifndef Joystick_h
#define Joystick_h

#include <Arduino.h>
#include <Element.h>

class Joystick : public Element
{
	public:
		Joystick(const String & name, bool snapBack, bool roundShape);
		Joystick(const String & name, bool snapBack, bool roundShape, const String & style, const String & id = "", const String & classList = "");

		void setSnapBack(bool snapBack, double newSnapX = 0, double newSnapY = 0);
		void setLabels(const String & top, const String & right, const String & bottom, const String & left);
		void setUpdateCallback(void (*updateallback)(double, double));

		double getX();
		double getY();

	private:
		double xValue = 0;
		double yValue = 0;
		void (*updateallback)(double, double);

		void connected(uint8_t num);
		void data(uint8_t num, const String & data);
		void disconnected(uint8_t num);
};

#endif