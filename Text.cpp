#include <Text.h>

Text::Text(String name)
{
	setInitJs("");
	setUpdateJs("element.innerText = message");
	setName(name);
}

Text::Text(String name, String style, String id, String classList)
{
	setInitJs("element.id = '" + id + "'; element.style = '" + style + "'; element.classList = '" + classList + "';");
	setUpdateJs("element.innerText = message");
	setName(name);
}

void Text::setText(String newText)
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

void Text::data(uint8_t num, String data)
{

}

void Text::disconnected(uint8_t num)
{

}