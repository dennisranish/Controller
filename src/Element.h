#ifndef Element_h
#define Element_h

class Element;
class Controller;
class SingleController;

#include <Arduino.h>
#include <SharedMemory.h>

class Element
{
	public:
		char * name = NULL;
		Element * parent = NULL;
		SingleController * parentSingleController = NULL;
		Controller * parentController = NULL;
		
		void display(bool visible);

	protected:
		std::vector<char*> js;
		char* defualDisplay = "block";
		std::vector<Element*> children;

		void selectSelf(uint8_t num);
		void broadcastSelectSelf();

		void addChild(Element * element);
		void removeChild(Element * element);

		void parseData(uint8_t num, char * data);

		virtual void connectedEvent(uint8_t num) = 0;
		virtual void dataEvent(uint8_t num, char * data) = 0;
		virtual void disconnectedEvent(uint8_t num) = 0;
		void sendData(uint8_t num, char * data);
		void broadcastData(char * data);

		void sendRun(uint8_t num);
		void broadcastRun();

		static void callConnectedEvent(Element * element, uint8_t num) {element->connectedEvent(num);}
		static void callDataEvent(Element * element, uint8_t num, char * data) {element->dataEvent(num, data);}
		static void callDisconnectedEvent(Element * element, uint8_t num) {element->disconnectedEvent(num);}

	private:
		void setController(Controller * newParentController);
		void setSingleController(SingleController * newParentSingleController);

		void select(uint8_t num, Element * element);
		virtual void customSelect(uint8_t num, Element * element);
		void broadcastSelect(Element * element);
		virtual void customBroadcastSelect(Element * element);
		
		friend class Controller;
		friend class SingleController;
};

#endif