#include "SingleController.h"

SingleController::SingleController(int index, Controller* controller)
{
	thisControllerIndex = index;
	parentController = controller;
}

void SingleController::add(Element *addElement)
{
	element.push_back(addElement);
	if(addElement->parentPage != NULL) addElement->parentPage->remove(addElement);
	addElement->parentPage = this;
	webSocket.broadcastTXT("2," + String(thisControllerIndex) + "," + String(element.size() - 1) + "," + String(element[element.size() - 1]->initJs));
	webSocket.braodcastTXT("3," + String(thisControllerIndex) + "," + String(element.size() - 1) + "," + String(element[element.size() - 1]->updateJs));
}

Element* SingleController::e(int index)
{
	return element[index];
}

/*template <typename T> T* SingleController::e(int index)
{
	return (T*)element[index];
}*/

Element* SingleController::e(const String & name)
{
	int index = -1;
	for(int i = 0; i < element.size(); i++) if(element[i]->name == name) { index = i; break; }
	if(index == -1) return NULL;
	return element[index];
}

/*template <typename T> T* SingleController::e(const String & name)
{
	int index = -1;
	for(int i = 0; i < element.size(); i++) if(element[i]->name == name) { index = i; break; }
	if(index == -1) return NULL;
	return (T*)element[index];
}*/

void SingleController::remove(Element *removeElement)
{
	for(int i = 0; i < element.size(); i++) if(element[i] == removeElement) element.erase(element.begin() + i--);
}

void SingleController::sendData(uint8_t num, Element *elementId, const String & data)
{
	for(int i = 0; i < element.size(); i++)
	{
		if(element[i] == elementId) parentController->webSocket.sendTXT(num, "1," + String(thisControllerIndex) + "," + String(i) + "," + data);
	}
}

void SingleController::broadcastData(Element *elementId, const String & data)
{
	for(int i = 0; i < element.size(); i++)
	{
		if(element[i] == elementId) parentController->webSocket.broadcastTXT("1," + String(thisControllerIndex) + "," + String(i) + "," + data);
	}
}