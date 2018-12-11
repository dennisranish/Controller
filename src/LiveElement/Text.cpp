#include "Text.h"

Text::Text(const String & name)
{
	setInitJs("");
	setUpdateJs("element.innerText = message");
	setName(name);
}

Text::Text(const String & name, const String & style, const String & id, const String & classList)
{
	setInitJs("element.id = '" + id + "'; element.style = '" + style + "'; element.classList = '" + classList + "';");
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