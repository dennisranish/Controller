#include "Controller.h"

String Controller::webPage0 = "<!DOCTYPE html>\n<html>\n<head>\n<meta name=viewport content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n<style>body{margin:0}.element{border:1px solid black;margin:10px}</style>\n</head>\n<body>\n<script>function run(code,element,message){eval(code);}\nvar element=[];var webSocket=new WebSocket('ws://' + window.location.hostname  + ':";
String Controller::webPage1 = "/');webSocket.onmessage=function(a)\n{var parameter=a.data.split(',');var type=parseInt(parameter[0]);if(type==0||type==1||type==2)\n{var index=parseInt(parameter[1]);var message=a.data.substring(a.data.indexOf(',',a.data.indexOf(',')+1)+1);if(type==0&&element[index]!=undefined)run(element[index].update,element[index].div,message);if(type==1&&element[index]!=undefined)\n{element[index].div.remove();element[index]=undefined;}\nif(element[index]==undefined)\n{element[index]={init:'',div:document.createElement('div'),update:''};element[index].div.sendData=(data)=>webSocket.send('0,'+index+','+data);element[index].div.classList='element';document.body.append(element[index].div);}\nif(type==1)run(element[index].init=message,element[index].div);if(type==2)element[index].update=message;}}</script>\n</body>\n</html>";

Controller::Controller(int count, int setPort) : webSocket(setPort)
{
	port = setPort;
	webPage = webPage0 + String(port) + webPage1;
	webSocket.onEvent([=](uint8_t num, WStype_t type, uint8_t * payload, size_t length){ webSocketEvent(num, type, payload, length); });
	webSocket.begin();

	for(int i = 0; i < count; i++) singleController.push_back(new SingleController(i, this));
}

const char* Controller::getWebPage()
{
	return webPage.c_str();
}

void Controller::update()
{
	webSocket.loop();
}

SingleController& Controller::operator[] (int index)
{
	return *singleController[index];
}

void Controller::webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
	/*Server-Sent
	0 - controllerCount (0 , count)
	1 - message (1 , controllerIndex , elementIndex , message)
	2 - initJs (2 , controllerIndex , elementIndex , initJs)
	3 - updateJs (3 , controllerIndex , elementIndex , updateJs)
	4 - noOwner (4 , controllerIndex)
	5 - yesOwner (5 , controllerIndex)
	6 - yourOwner (6 , controllerIndex)
	*/
	/*Client-Sent
	0 - message (0 , controllerIndex , elementIndex , message)
	1 - takeController (1 , controllerIndex)
	*/

	if (type == WStype_CONNECTED)
	{
		webSocket.sendTXT(num, "0," + String(singleController.size()));

		for(int c = 0; c < singleController.size(); c++)
		{
			if(singleController[c]->hasOwner == true) webSocket.sendTXT(num, "5," + String(c));
			else webSocket.sendTXT(num, "4," + String(c));

			for(int e = 0; e < singleController[c]->element.size(); e++)
			{
				webSocket.sendTXT(num, "2," + String(c) + "," + String(e) + "," + String(singleController[c]->element[e]->initJs));
				webSocket.sendTXT(num, "3," + String(c) + "," + String(e) + "," + String(singleController[c]->element[e]->updateJs));
				singleController[c]->element[e]->connected(num);
			}
		}
	}
	else if (type == WStype_DISCONNECTED)
	{
		for(int c = 0; c < singleController.size(); c++)
		{
			if(singleController[c]->hasOwner && singleController[c]->owner == num)
			{
				singleController[c]->hasOwner = false;
				webSocket.broadcastTXT("4," + String(c));
			}

			for(int e = 0; e < singleController[c]->element.size(); e++)
			{
				singleController[c]->element[e]->disconnected(num);
			}
		}
	}
	else if (type == WStype_TEXT)
	{
		if(length == 0) return;
		char *message = (char*)malloc(length + 1);
		for (int i = 0; i < length; i++) message[i] = payload[i];
		message[length] = '\0';

		char *nextNumber = message;

		int messageType = strtol(nextNumber, &nextNumber, 10);
		nextNumber++;

		if(messageType == 0)
		{
			if (nextNumber == NULL) return;
			int controllerIndex = strtol(nextNumber, &nextNumber, 10);
			nextNumber++;

			if(!singleController[controllerIndex]->hasOwner || singleController[controllerIndex]->owner != num) return;

			if (nextNumber == NULL) return;
			int elementIndex = strtol(nextNumber, &nextNumber, 10);
			nextNumber++;

			if(elementIndex < singleController[controllerIndex]->element.size() && elementIndex >= 0)
			{
				singleController[controllerIndex]->element[elementIndex]->data(num, String(nextNumber));
			}
		}
		else if(messageType == 1)
		{
			if (nextNumber == NULL) return;
			int controllerIndex = strtol(nextNumber, &nextNumber, 10);
			nextNumber++;

			//if(singleController[controllerIndex]->hasOwner && singleController[controllerIndex]->owner == num) return;

			singleController[controllerIndex]->hasOwner = true;
			singleController[controllerIndex]->owner = num;

			webSocket.broadcastTXT("5," + String(controllerIndex));
			webSocket.sendTXT(num, "6," + String(controllerIndex));
		}

		free(message);
	}
}