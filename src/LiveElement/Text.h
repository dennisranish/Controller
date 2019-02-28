#ifndef Text_h
#define Text_h

#include <Arduino.h>
#include <Element.h>

class Text : public Element
{
	public:
		Text(char * name, char * style = "");

		void setText(char* newText);
		const char * getText();

	private:
		char * text = "";
		char * style = "";

		void connectedEvent(uint8_t num);
		void dataEvent(uint8_t num, char * data);
		void disconnectedEvent(uint8_t num);
};

#endif