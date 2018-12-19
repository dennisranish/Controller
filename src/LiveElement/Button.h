#ifndef Button_h
#define Button_h

#include <Arduino.h>
#include <Element.h>
#include <FastString.h>

class Button : public Element
{
	public:
		Button(char* name, char* title);
		Button(char* name, char* title, char* style);

		void setTitle(char* newTitle);
		const char* getTitle();
		bool getValue();
		void setChangeCallback(void (*newChangeCallback)(bool));
		void setPressedCallback(void (*newPressedCallback)());
		void setUnpressedCallback(void (*newUnpressedCallback)());

	private:
		static char* jsInitCode;
		static char* jsUpdateCode;
		char* title;
		bool value;
		void (*changeCallback)(bool);
		void (*pressedCallback)();
		void (*unpressedCallback)();

		void connected(uint8_t num);
		void data(uint8_t num, char* data);
		void disconnected(uint8_t num);
};

#endif