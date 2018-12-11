#ifndef SingleController_h
#define SingleController_h

class SingleController;

#include <Arduino.h>
#include <WebSocketsServer.h>
#include <Controller.h>
#include <Element.h>

class SingleController
{
	public:
		SingleController(int index, Controller* controller);
		void add(Element *addElement);
		Element* e(int index);
		template <typename T> T* e(int index)
		{
			return (T*)element[index];
		}
		Element* e(const String & name);
		template <typename T> T* e(const String & name)
		{
			int index = -1;
			for(int i = 0; i < element.size(); i++) if(element[i]->name == name) { index = i; break; }
			if(index == -1) return NULL;
			return (T*)element[index];
		}
		void remove(Element *removeElement);

	private:
		int thisControllerIndex;
		Controller* parentController;
		std::vector<Element*> element;

		bool hasOwner = false;
		uint8_t owner = 0;

		void sendData(uint8_t num, Element *elementId, const String & data);
		void broadcastData(Element *elementId, const String & data);

		friend class Element;
		friend class Controller;
};

#endif