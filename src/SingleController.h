#ifndef SingleController_h
#define SingleController_h

class SingleController;

#include <Arduino.h>
#include <WebSocketsServer.h>
#include <Controller.h>
#include <Element.h>
#include <SharedMemory.h>

class SingleController : public Element
{
	public:
		bool getHasOwner();
		uint8_t getOwner();

		SingleController(int index, Controller * controller);
		void add(Element * element);
		template <typename T = Element> T * e(int index);
		template <typename T = Element> T * e(char * name);
		void remove(Element * element);

	private:
		int controllerIndex;

		bool hasOwner = false;
		uint8_t owner = 0;

		void connectedEvent(uint8_t num);
		void dataEvent(uint8_t num, char * data);
		void disconnectedEvent(uint8_t num);

		void customSelect(uint8_t num, Element * element);
		void customBroadcastSelect(Element * element);

		friend class Controller;
};

#endif