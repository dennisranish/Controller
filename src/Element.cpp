#include "Element.h"
#include "Controller.h"

void Element::display(bool visible)
{

}

void Element::selectSelf(uint8_t num)
{
	if(parent == NULL) return;
	parent->customSelect(num, this);
}

void Element::broadcastSelectSelf()
{
	if(parent == NULL) return;
	parent->customBroadcastSelect(this);
}

void Element::select(uint8_t num, Element * element)
{
	parent->customSelect(num, this);
	for(int i = 0; i < children.size(); i++) if(children[i] == element)
	{
		sprintf(SharedMemory::buffer, "\002%i", i);
		sendData(num, SharedMemory::buffer);
		break;
	}
}

void Element::customSelect(uint8_t num, Element * element)
{
	select(num, element);
}

void Element::broadcastSelect(Element * element)
{
	parent->customBroadcastSelect(this);
	for(int i = 0; i < children.size(); i++) if(children[i] == element)
	{
		sprintf(SharedMemory::buffer, "\002%i", i);
		broadcastData(SharedMemory::buffer);
		break;
	}
}

void Element::customBroadcastSelect(Element * element)
{
	broadcastSelect(element);
}

void Element::addChild(Element * element)
{
	children.push_back(element);
	if(element->parent != NULL) element->parent->removeChild(element);
	element->parent = this;
	element->setController(parentController);
	element->setSingleController(parentSingleController);
}

void Element::removeChild(Element * element)
{
	for(int i = 0; i < children.size(); i++) if(children[i] == element) children.erase(children.begin() + i--);
}

void Element::parseData(uint8_t num, char * data)
{
	if(data[0] == 1)
	{
		int childIndex = strtol(++data, &data, 10);
		if(children.size() <= childIndex) return;
		children[childIndex]->parseData(num, data);
	}
	else
	{
		dataEvent(num, data);
	}
	
}

void Element::sendData(uint8_t num, char * data)
{
	parentController->webSocket.sendTXT(num, data);
}

void Element::broadcastData(char * data)
{
	parentController->webSocket.broadcastTXT(data);
}

void Element::sendRun(uint8_t num)
{
	sendData(num, "\003");
}

void Element::broadcastRun()
{
	broadcastData("\003");
}

void Element::setController(Controller * newParentController)
{
	parentController = newParentController;
	for(int i = 0; i < children.size(); i++) children[i]->setController(newParentController);
}

void Element::setSingleController(SingleController * newParentSingleController)
{
	parentSingleController = newParentSingleController;
	for(int i = 0; i < children.size(); i++) children[i]->setSingleController(newParentSingleController);
}