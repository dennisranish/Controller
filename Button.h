#ifndef Button_h
#define Button_h

#include <Arduino.h>
#include <Element.h>

class Button : public Element
{
	public:
		Button(const String & name, const String & title);
		Button(const String & name, const String & title, const String & style, const String & id = "", const String & classList = "");

		void setTitle(const String & newTitle);
		String getTitle();
		bool getValue();
		void setChangeCallback(void (*newChangeCallback)(bool));
		void setPressedCallback(void (*newPressedCallback)());
		void setUnpressedCallback(void (*newUnpressedCallback)());

	private:
		String title;
		bool value;
		void (*changeCallback)(bool);
		void (*pressedCallback)();
		void (*unpressedCallback)();

		void connected(uint8_t num);
		void data(uint8_t num, const String & data);
		void disconnected(uint8_t num);
};

#endif