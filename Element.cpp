#include "Element.h"

void Element::setInitJs(String newInitJs)
{
	initJs = newInitJs;
}

void Element::setUpdateJs(String newUpdateJs)
{
	updateJs = newUpdateJs;
}

void Element::setName(String newName)
{
	name = newName;
}

void Element::sendData(uint8_t num, String data)
{
	for(int i = 0; i < parentPage.size(); i++)
	{
		parentPage[i]->sendData(num, this, data);
	}
}

void Element::broadcastData(String data)
{
	for(int i = 0; i < parentPage.size(); i++)
	{
		parentPage[i]->broadcastData(this, data);
	}
}