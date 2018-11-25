#ifndef Controller_h
#define Controller_h

#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <vector>

class Controller
{
	public:
		static void sendWebPage(ESP8266WebServer server);
		static void init();
		static void loop();

		Controller();
		void addOption(char *name, void(*callback)(int, char*));
		void addOption(char *name, std::initializer_list<char*> option, void(*callback)(int, char*));

	private:
		static WebSocketsServer webSocket(141);
		static const char *webPage = "";
		static std::vector<Controller*> ControllerList;

		static void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

		bool ownerExists;
		uint8_t owner;
		double joystick[4];

		static const int maxOptions;
		std::vector<char*> optionName;
		std::vector<std::vector<char*>> optionOption;
		std::vector<void(*callback)(int, char*)> optionCallback;
		std::vector<int> optionState;
};

#endif