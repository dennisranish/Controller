#ifndef Joystick_h
#define Joystick_h

#include <Arduino.h>
#include <Element.h>

class Joystick : public Element
{
	public:
		Joystick(const String & name);
		Joystick(const String & name, const String & style);

		void setTitle(const String & newTitle);
		void setLabels(const String & newTopLabel, const String & newRightLabel, const String & newBottomLabel, const String & newLeftLabel);
		void setUpdateCallback(void (*newUpdateCallback)(double, double));

		double x();
		double y();

	private:
		static String jsInitCode;
		static String jsUpdateCode;
		double xValue = 0;
		double yValue = 0;
		String title;
		String topLabel, rightLabel, bottomLabel, leftLabel;
		void (*updateCallback)(double, double);

		void connected(uint8_t num);
		void data(uint8_t num, const String & data);
		void disconnected(uint8_t num);
};

#endif