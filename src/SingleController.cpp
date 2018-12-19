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
	parentController->webSocket.broadcastTXT(FastString::add({ (char*)"2,", (char*)String(thisControllerIndex).c_str(), (char*)",", (char*)String(element.size() - 1).c_str(), (char*)",", element[element.size() - 1]->initJs }));
	parentController->webSocket.broadcastTXT(FastString::add({ (char*)"3,", (char*)String(thisControllerIndex).c_str(), (char*)",", (char*)String(element.size() - 1).c_str(), (char*)",", element[element.size() - 1]->updateJs }));
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
			parentController->webSocket.sendTXT(num, FastString::add({ (char*)"1,", (char*)String(thisControllerIndex).c_str(), (char*)",", (char*)String(i).c_str(), (char*)",", data }));
		}
	}
}

void SingleController::broadcastData(Element *elementId, char* data)
{
	for(int i = 0; i < element.size(); i++)
	{
		if(element[i] == elementId)
		{
			parentController->webSocket.broadcastTXT(FastString::add({ (char*)"1,", (char*)String(thisControllerIndex).c_str(), (char*)",", (char*)String(i).c_str(), (char*)",", data }));
		}
	}
}