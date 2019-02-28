#ifndef Button_h
#define Button_h

#include <Arduino.h>
#include <Element.h>

class Button : public Element
{
	public:
		Button(char * name, char * title, char * style = "");

		void setTitle(char * newTitle);
		const char * getTitle();
		bool getValue();
		void setChangeCallback(void (*newChangeCallback)(bool));
		void setPressedCallback(void (*newPressedCallback)());
		void setUnpressedCallback(void (*newUnpressedCallback)());

	private:
		static char * jsCode;
		char * style = "";
		char * title;
		bool value = false;
		void (*changeCallback)(bool);
		void (*pressedCallback)();
		void (*unpressedCallback)();

		void connectedEvent(uint8_t num);
		void dataEvent(uint8_t num, char * data);
		void disconnectedEvent(uint8_t num);
};

#endif