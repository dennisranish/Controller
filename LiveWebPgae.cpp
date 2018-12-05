#include "LiveWebPage.h"

String LiveWebPage::webPage0 = "<!DOCTYPE html>\n<html>\n<head>\n<meta name=viewport content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n<style>body{margin:0}.element{border:1px solid black;margin:10px}</style>\n</head>\n<body>\n<script>function run(code,element,message){eval(code);}\nvar element=[];var webSocket=new WebSocket('ws://' + window.location.hostname  + ':";
String LiveWebPage::webPage1 = "/');webSocket.onmessage=function(a)\n{var parameter=a.data.split(',');var type=parseInt(parameter[0]);if(type==0||type==1||type==2)\n{var index=parseInt(parameter[1]);var message=a.data.substring(a.data.indexOf(',',a.data.indexOf(',')+1)+1);if(type==0&&element[index]!=undefined)run(element[index].update,element[index].div,message);if(type==1&&element[index]!=undefined)\n{element[index].div.remove();element[index]=undefined;}\nif(element[index]==undefined)\n{element[index]={init:'',div:document.createElement('div'),update:''};element[index].div.sendData=(data)=>webSocket.send('0,'+index+','+data);element[index].div.classList='element';document.body.append(element[index].div);}\nif(type==1)run(element[index].init=message,element[index].div);if(type==2)element[index].update=message;}}</script>\n</body>\n</html>";

LiveWebPage::LiveWebPage(int setPort) : webSocket(setPort)
{
	port = setPort;
	webPage = webPage0 + String(port) + webPage1;
	webSocket.onEvent([=](uint8_t num, WStype_t type, uint8_t * payload, size_t length){ webSocketEvent(num, type, payload, length); });
	webSocket.begin();
}

void LiveWebPage::add(Element *addElement)
{
	element.push_back(addElement);
	addElement->parentPage.push_back(this);
	webSocket.broadcastTXT("1," + String(element.size() - 1) + "," + String(element[element.size() - 1]->initJs));
	webSocket.broadcastTXT("2," + String(element.size() - 1) + "," + String(element[element.size() - 1]->updateJs));
}

Element* LiveWebPage::e(int index)
{
	return element[index];
}

/*template <typename T> T* LiveWebPage::e(int index)
{
	return (T*)element[index];
}*/

Element* LiveWebPage::e(String name)
{
	int index = -1;
	for(int i = 0; i < element.size(); i++) if(element[i]->name == name) { index = i; break; }
	if(index == -1) return NULL;
	return element[index];
}

/*template <typename T> T* LiveWebPage::e(String name)
{
	int index = -1;
	for(int i = 0; i < element.size(); i++) if(element[i]->name == name) { index = i; break; }
	if(index == -1) return NULL;
	return (T*)element[index];
}*/

const char* LiveWebPage::getWebPage()
{
	return webPage.c_str();
}

void LiveWebPage::update()
{
	webSocket.loop();
}

void LiveWebPage::webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
	/*Server-Sent
	0 - message (0 , elementIndex, message)
	1 - initJs (1 , elementIndex, initJs)
	2 - updateJs (2 , elementIndex, updateJs)
	*/
	/*Client-Sent
	0 - message (0, elementIndex, message)
	*/

	if (type == WStype_CONNECTED)
	{
		for(int i = 0; i < element.size(); i++)
		{
			webSocket.sendTXT(num, "1," + String(i) + "," + String(element[i]->initJs));
			webSocket.sendTXT(num, "2," + String(i) + "," + String(element[i]->updateJs));
			element[i]->connected(num);
		}
	}
	else if (type == WStype_DISCONNECTED)
	{
		for(int i = 0; i < element.size(); i++) element[i]->disconnected(num);
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
			int elementIndex = strtol(nextNumber, &nextNumber, 10);
			nextNumber++;

			if(elementIndex < element.size() && elementIndex >= 0) element[elementIndex]->data(num, String(nextNumber));
		}

		free(message);
	}
}

void LiveWebPage::sendData(uint8_t num, Element *elementId, String data)
{
	for(int i = 0; i < element.size(); i++)
	{
		if(element[i] == elementId) webSocket.sendTXT(num, "0," + String(i) + "," + data);
	}
}

void LiveWebPage::broadcastData(Element *elementId, String data)
{
	for(int i = 0; i < element.size(); i++)
	{
		if(element[i] == elementId) webSocket.broadcastTXT("0," + String(i) + "," + data);
	}
}