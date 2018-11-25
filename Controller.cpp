#include "Controller.h"

static void Controller::sendWebPage(ESP8266WebServer server)
{
	server.send_P(200, "text/html", webPage);
}

static void Controller::init()
{
	webSocket.onEvent(webSocketEvent);
	webSocket.begin();
}

static void Controller::loop()
{
	webSocket.loop();
}

static void Controller::webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
	/* server sent
	 * 
	 * controllerCount (count)
	 * controller (index \0 name \0 [option \0])
	 * controllerOwnershipUpdate (index, isTaken)
	 * youLostController (index)
	 * youGainedController (index)
	 * joystickUpdate (index, joystick, value)
	 * optionUpdate (index, optionIndex, value)
	 */

	/* client sent
	 * 
	 * takeController (index)
	 * joystickUpdate (index, joystick, value)
	 * optionUpdate (index, optionIndex, value)
	 */

	if (type == WStype_CONNECTED)
	{
	}
	else if (type == WStype_DISCONNECTED)
	{
	}
	else if (type == WStype_TEXT)
	{
	}
}

Controller::Controller()
{
	ControllerList.push_back(&this);
}

void Controller::addOption(char *name, void(*callback)(int, char*) = [](int, char*){})
{
	if (optionName.size() >= maxOptions) return;

	optionName.push_back(name);
	optionOption.push_back(std::vector<char*>());
	optionCallback.push_back(callback);
	optionState.push_back(0);
}

void Controller::addOption(char *name, std::initializer_list<char*> option, void(*callback)(int, char*) = [](int, char*) {})
{
	if (optionName.size() >= maxOptions) return;

	optionName.push_back(name);
	optionOption.push_back((std::vector<char*>)option);
	optionCallback.push_back(callback);
	optionState.push_back(0);
}