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
	initJs.clear();
	initJs.push_back(newInitJs);
}

void Element::setUpdateJs(char* newUpdateJs)
{
	updateJs.clear();
	updateJs.push_back((char*)"if(message.charCodeAt(0) == 1) { eval(message.substring(1)); } else {");
	updateJs.push_back(newUpdateJs);
	updateJs.push_back((char*)"}");
}

void Element::setName(char* newName)
{
	name = newName;
}

void Element::setInitJs(std::vector<char*> newInitJs)
{
	initJs.insert(initJs.end(), newInitJs.begin(), newInitJs.end());
}

void Element::setUpdateJs(std::vector<char*> newUpdateJs)
{
	updateJs.clear();
	updateJs.push_back((char*)"if(message.charCodeAt(0) == 1) { eval(message.substring(1)); } else {");
	updateJs.insert(updateJs.end(), newUpdateJs.begin(), newUpdateJs.end());
	updateJs.push_back((char*)"}");
}

void Element::sendData(uint8_t num, char* data)
{
	parentController->sendData(num, this, data);
}

void Element::broadcastData(char* data)
{
	parentController->broadcastData(this, data);
}