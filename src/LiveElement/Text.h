#ifndef Text_h
#define Text_h

#include <Arduino.h>
#include <Element.h>

class Text : public Element
{
	public:
		Text(const String & name);
		Text(const String & name, const String & style);

		void setText(const String & newText);
		String getText();

	private:
		String text;

		void connected(uint8_t num);
		void data(uint8_t num, const String & data);
		void disconnected(uint8_t num);
};

#endif