#ifndef Controller_h
#define Controller_h

class Controller;

#include <Arduino.h>
#include <WebSocketsServer.h>
#include <SingleController.h>
#include <Element.h>

class Controller
{
	public:
		Controller(int count, int setPort);
		Controller(std::initializer_list<char*> nameList, int setPort);
		const char* getWebPage();
		void update();
		SingleController& operator[] (int index);

	private:
		int port;
		static char* webPage0;
		static char* webPage1;
		char* webPage;
		WebSocketsServer webSocket;
		std::vector<SingleController*> singleController;
		std::vector<char*> singleControllerName;

		void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

		friend class Element;
		friend class SingleController;
};

#endif