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
		const char* getWebPage();
		void update();
		SingleController& operator[] (int index);

	private:
		int port;
		static String webPage0;
		static String webPage1;
		String webPage;
		WebSocketsServer webSocket;
		std::vector<SingleController*> singleController;

		void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

		friend class Element;
		friend class SingleController;
};

#endif