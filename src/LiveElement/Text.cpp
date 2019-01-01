#include "Text.h"

Text::Text(char* name)
{
	setInitJs("element.style = 'border: 1px solid black; margin: 10px; padding: 10px;';");
	setUpdateJs("element.innerText = message");
	setName(name);
}

Text::Text(char* name, char* style)
{
	setInitJs({ (char*)"element.style = '", style, (char*)"border: 1px solid black; margin: 10px; padding: 10px;';" });
	setUpdateJs("element.innerText = message");
	setName(name);
}

void Text::setText(char* newText)
{
	text = newText;
	broadcastData(text);
}

const char* Text::getText()
{
	return text;
}

void Text::connected(uint8_t num)
{

}

void Text::data(uint8_t num, char* data)
{

}

void Text::disconnected(uint8_t num)
{

}