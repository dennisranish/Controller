#include "Element.h"
#include "Controller.h"
#include "SingleController.h"

void Element::display(bool visible)
{
	if(visible) broadcastData("\x001element.style.display = 'block';");
	else broadcastData("\x001element.style.display = 'none';");
}

void Element::display(char* type)
{
	char* stringA = FastString::add({ (char*)"\x001element.style.display = '", type, (char*)"';" });
	broadcastData(stringA);
	free(stringA);
}

void Element::setInitJs(char* newInitJs)
{
	initJs = FastString::add({ newInitJs });
}

void Element::setUpdateJs(char* newUpdateJs)
{
	updateJs = FastString::add({ (char*)"if(message.charCodeAt(0) == 1) { eval(message.substring(1);) } else {", newUpdateJs, (char*)"}" });
}

void Element::setName(char* newName)
{
	name = FastString::add({ newName });
}

void Element::sendData(uint8_t num, char* data)
{
	parentController->sendData(num, this, data);
}

void Element::broadcastData(char* data)
{
	parentController->broadcastData(this, data);
}