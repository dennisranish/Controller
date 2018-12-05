#ifndef Button_h
#define Button_h

#include <Arduino.h>
#include <Element.h>

class Button : public Element
{
	public:
		Button(String name, String title);
		Button(String name, String title, String style, String id = "", String classList = "");

		void setTitle(String newTitle);
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
		void data(uint8_t num, String data);
		void disconnected(uint8_t num);
};

#endif