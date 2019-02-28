#include "SingleController.h"

bool SingleController::getHasOwner()
{
	return hasOwner;
}

uint8_t SingleController::getOwner()
{
	return owner;
}

SingleController::SingleController(int index, Controller * controller)
{
	controllerIndex = index;
	parentSingleController = this;
	parentController = controller;
}

void SingleController::add(Element * element)
{
	addChild(element);
}

template <typename T> T * SingleController::e(int index)
{
	return (T*)children[index];
}

template <typename T> T * SingleController::e(char * name)
{
	for(int i = 0; i < children.size(); i++) if(children[i]->name == name) return (T*)children[i];
	for(int i = 0; i < children.size(); i++) if(strcmp(children[i]->name, name) == 0) return (T*)children[i];
	return NULL;
}

void SingleController::remove(Element * element)
{
	removeChild(element);
}

void SingleController::connectedEvent(uint8_t num)
{
	if(hasOwner == true)
	{
		sprintf(SharedMemory::buffer, "\001%i", controllerIndex);
		sendData(num, SharedMemory::buffer);

		sendData(num, "element.setOwner(2);");
		sendRun(num);
	}

	for(int i = 0; i < children.size(); i++) callConnectedEvent(children[i], num);
}

void SingleController::dataEvent(uint8_t num, char * data)
{
	sprintf(SharedMemory::buffer, "\001%i", controllerIndex);
	broadcastData(SharedMemory::buffer);

	broadcastData("element.setOwner(2);");
	broadcastRun();

	sendData(num, "element.setOwner(1);");
	sendRun(num);

	hasOwner = true;
	owner = num;
}

void SingleController::disconnectedEvent(uint8_t num)
{
	if(hasOwner && owner == num)
	{
		hasOwner = false;

		sprintf(SharedMemory::buffer, "\001%i", controllerIndex);
		broadcastData(SharedMemory::buffer);

		broadcastData("element.setOwner(0);");
		broadcastRun();
	}

	for(int i = 0; i < children.size(); i++) callDisconnectedEvent(children[i], num);
}

void SingleController::customSelect(uint8_t num, Element * element)
{
	sprintf(SharedMemory::buffer, "\001%i", controllerIndex);
	sendData(num, SharedMemory::buffer);

	for(int i = 0; i < children.size(); i++) if(children[i] == element)
	{
		sprintf(SharedMemory::buffer, "\002%i", i);
		sendData(num, SharedMemory::buffer);
		break;
	}
}

void SingleController::customBroadcastSelect(Element * element)
{
	sprintf(SharedMemory::buffer, "\001%i", controllerIndex);
	broadcastData(SharedMemory::buffer);

	for(int i = 0; i < children.size(); i++) if(children[i] == element)
	{
		sprintf(SharedMemory::buffer, "\002%i", i);
		broadcastData(SharedMemory::buffer);
		break;
	}
}