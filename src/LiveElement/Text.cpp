#include "Text.h"

Text::Text(const String & name)
{
	setInitJs("element.style = 'border: 1px solid black; margin: 10px; padding: 10px;';");
	setUpdateJs("element.innerText = message");
	setName(name);
}

Text::Text(const String & name, const String & style)
{
	setInitJs("element.style = '" + style + "border: 1px solid black; margin: 10px; padding: 10px;';");
	setUpdateJs("element.innerText = message");
	setName(name);
}

void Text::setText(const String & newText)
{
	text = newText;
	broadcastData(text);
}

String Text::getText()
{
	return text;
}

void Text::connected(uint8_t num)
{

}

void Text::data(uint8_t num, const String & data)
{

}

void Text::disconnected(uint8_t num)
{

}