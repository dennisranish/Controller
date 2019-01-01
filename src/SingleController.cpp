#include "SingleController.h"

SingleController::SingleController(int index, Controller* controller)
{
	thisControllerIndex = index;
	parentController = controller;
}

void SingleController::add(Element *addElement)
{
	element.push_back(addElement);
	if(addElement->parentController != NULL) addElement->parentController->remove(addElement);
	addElement->parentController = this;
}

Element* SingleController::e(int index)
{
	return element[index];
}

/*template <typename T> T* SingleController::e(int index)
{
	return (T*)element[index];
}*/

Element* SingleController::e(char* name)
{
	int index = -1;
	for(int i = 0; i < element.size(); i++) if(strcmp(element[i]->name, name) == 0) { index = i; break; }
	if(index == -1) return NULL;
	return element[index];
}

/*template <typename T> T* SingleController::e(char* name)
{
	int index = -1;
	for(int i = 0; i < element.size(); i++) if(strcmp(element[i]->name, name) == 0) { index = i; break; }
	if(index == -1) return NULL;
	return (T*)element[index];
}*/

void SingleController::remove(Element *removeElement)
{
	for(int i = 0; i < element.size(); i++) if(element[i] == removeElement) element.erase(element.begin() + i--);
}

void SingleController::sendData(uint8_t num, Element *elementId, char* data)
{
	for(int i = 0; i < element.size(); i++)
	{
		if(element[i] == elementId)
		{
			char* stringA = FastString::add({ (char*)"1,", (char*)String(thisControllerIndex).c_str(), (char*)",", (char*)String(i).c_str(), (char*)",", data });
			parentController->webSocket.sendTXT(num, stringA);
			free(stringA);
		}
	}
}

void SingleController::broadcastData(Element *elementId, char* data)
{
	for(int i = 0; i < element.size(); i++)
	{
		if(element[i] == elementId)
		{
			char* stringA = FastString::add({ (char*)"1,", (char*)String(thisControllerIndex).c_str(), (char*)",", (char*)String(i).c_str(), (char*)",", data });
			parentController->webSocket.broadcastTXT(stringA);
			free(stringA);
		}
	}
}