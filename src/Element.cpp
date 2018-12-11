#include "Element.h"
#include "Controller.h"

void Element::display(bool visible)
{
	if(visible) broadcastData("\x001element.style.display = 'block';");
	else broadcastData("\x001element.style.display = 'none';");
}

void Element::display(const String & type)
{
	broadcastData("\x001element.style.display = '" + type + "';");
}

void Element::setInitJs(const String & newInitJs)
{
	initJs = newInitJs;
}

void Element::setUpdateJs(const String & newUpdateJs)
{
	updateJs = "if(message.charCodeAt(0) == 1) { eval(message.substring(1);) } else {" + newUpdateJs + "}";
}

void Element::setName(const String & newName)
{
	name = newName;
}

void Element::sendData(uint8_t num, const String & data)
{
	parentController->sendData(num, this, data);
}

void Element::broadcastData(const String & data)
{
	parentController->broadcastData(this, data);
}