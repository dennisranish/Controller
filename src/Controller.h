#ifndef Controller_h
#define Controller_h

class Controller;

#include <Arduino.h>
#include <WebSocketsServer.h>
#include <SingleController.h>
#include <Element.h>
#include <SharedMemory.h>

class Controller
{
	public:
		Controller(int count, int setPort);
		Controller(std::initializer_list<char*> nameList, int setPort);
		char* getWebPage();
		void update();
		SingleController& operator[] (int index);

	private:
		int port;
		static char * webPageOnMessage;
		static char * webPageOnOpen;
		static char * webPageOnClose;
		static char * webPageLoading;
		static char * fullscreenToggle;
		char* webPage;
		WebSocketsServer webSocket;
		std::vector<SingleController*> singleController;
		std::vector<char*> singleControllerName;

		void createWebPage();
		void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

		friend class Element;
		friend class SingleController;
};

#endif