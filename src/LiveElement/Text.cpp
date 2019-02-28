#include "Text.h"

Text::Text(char * name, char * style)
{
	this->name = name;
	this->style = style;
}

void Text::setText(char * newText)
{
	text = newText;

	broadcastSelectSelf();
	broadcastData("element.innerText = '");
	broadcastData(text);
	broadcastData("';");
	broadcastRun();
}

const char * Text::getText()
{
	return text;
}

void Text::connectedEvent(uint8_t num)
{
	selectSelf(num);
	sendData(num, "element.innerText = '");
	sendData(num, text);
	sendData(num, "';");
	sendData(num, "element.style = '");
	sendData(num, style);
	sendData(num, "';");
	sendRun(num);
}

void Text::dataEvent(uint8_t num, char * data)
{

}

void Text::disconnectedEvent(uint8_t num)
{

}