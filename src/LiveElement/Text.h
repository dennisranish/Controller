#ifndef Text_h
#define Text_h

#include <Arduino.h>
#include <Element.h>
#include <FastString.h>

class Text : public Element
{
	public:
		Text(char* name);
		Text(char* name, char* style);

		void setText(char* newText);
		const char* getText();

	private:
		char* text;

		void connected(uint8_t num);
		void data(uint8_t num, char* data);
		void disconnected(uint8_t num);
};

#endif