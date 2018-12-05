#ifndef Text_h
#define Text_h

#include <Arduino.h>
#include <Element.h>

class Text : public Element
{
	public:
		Text(String name);
		Text(String name, String style, String id = "", String classList = "");

		void setText(String newText);
		String getText();

	private:
		String text;

		void connected(uint8_t num);
		void data(uint8_t num, String data);
		void disconnected(uint8_t num);
};

#endif