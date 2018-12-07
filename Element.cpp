#include "Element.h"
#include "LiveWebPage.h"

void Element::setInitJs(const String & newInitJs)
{
	initJs = newInitJs;
}

void Element::setUpdateJs(const String & newUpdateJs)
{
	updateJs = newUpdateJs;
}

void Element::setName(const String & newName)
{
	name = newName;
}

void Element::sendData(uint8_t num, const String & data)
{
	parentPage->sendData(num, this, data);
}

void Element::broadcastData(const String & data)
{
	parentPage->broadcastData(this, data);
}