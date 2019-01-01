#include "Controller.h"

char* Controller::webPage0 = "<!DOCTYPE html>\n<html>\n<head>\n<meta name=viewport content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n<style>body{margin:0;-webkit-touch-callout:none;-webkit-user-select:none;-khtml-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none;overflow:hidden}#connectingPage{width:100vw;height:100vh;position:absolute;top:0;left:0;display:flex;background:white;z-index:1002;justify-content:center;align-items:center;flex-direction:column}#connectingPageSvg{width:150px;height:150px}#connectingPageText{font:bold 30px sans-serif}#connectingPageButton{font:bold 30px sans-serif;padding:15px;border-radius:200px;border:5px solid grey;color:white;background:#292929}#navBar{position:absolute;top:0;left:0;display:flex;justify-content:space-between;height:0;width:100vw}#fullscreen{height:80px;margin:10px;padding-left:36.88px;cursor:pointer;z-index:1003}#fullscreen path:nth-child(1){transform-origin:17.5% 17.5%;transition:.5s ease}#fullscreen path:nth-child(2){transform-origin:82.5% 17.5%;transition:.5s ease}#fullscreen path:nth-child(3){transform-origin:82.5% 82.5%;transition:.5s ease}#fullscreen path:nth-child(4){transform-origin:17.5% 82.5%;transition:.5s ease}.unfullscreen path:nth-child(1){transform:rotate3D(1,-1,0,180deg)}.unfullscreen path:nth-child(2){transform:rotate3D(1,1,0,180deg)}.unfullscreen path:nth-child(3){transform:rotate3D(1,-1,0,180deg)}.unfullscreen path:nth-child(4){transform:rotate3D(1,1,0,180deg)}#controller{height:80px;margin:10px;cursor:pointer;z-index:1001}#select{border-top:1vh solid black;height:fit-content;border-bottom:1vh solid black;max-height:98vh;overflow:auto;z-index:1001;padding-left:30px;margin-right:30px}#select div{padding:10px;font:bold 25px sans-serif;cursor:pointer;text-align:center;position:relative}#select div[status=free]{border:5px solid grey;background:grey;color:white}#select div[status=mine]{border:5px solid darkgreen;background:darkgreen;color:white}#select div[status=other]{border:5px solid darkred;background:darkred;color:white}#select div[status=free][selected=false]:hover{background:white;color:grey}#select div[status=mine][selected=false]:hover{background:white;color:darkgreen}#select div[status=other][selected=false]:hover{background:white;color:darkred}#select div[status=free][selected=true]{background:#292929}#select div[status=mine][selected=true]{background:#002500}#select div[status=other][selected=true]{background:#380000}#select div:after{border-top:1px solid white;border-bottom:1px solid white;content:\"\";position:absolute;top:-5px;left:-5px;bottom:-5px;right:-5px}#select div[selected=true]:before{border-right:8px solid #00a9ff;border-top:8px solid #00a9ff;content:\"\";position:absolute;top:12px;left:-36px;width:15px;height:15px;transform:rotate(45deg)}#page{position:absolute;top:0;left:0;width:100vw;height:100vh}.controllerPage{width:100vw;height:100vh;position:absolute;top:0;left:0}.controllerPageOverlay{width:100vw;height:100vh;position:absolute;top:0;left:0;z-index:1000;justify-content:center;align-items:center}.controllerPageOverlayButton{padding:15px;font:bold 30px sans-serif;border-radius:200px}.controllerPageOverlayButton[status=free]{border:5px solid grey;background:#292929;color:white}.controllerPageOverlayButton[status=mine]{border:5px solid darkgreen;background:#002500;color:white}.controllerPageOverlayButton[status=other]{border:5px solid darkred;background:#380000;color:white}.controllerPageOverlayButton[status=free]:hover{background:white;color:grey}.controllerPageOverlayButton[status=mine]:hover{background:white;color:darkgreen}.controllerPageOverlayButton[status=other]:hover{background:white;color:darkred}</style>\n</head>\n<body>\n<div id=connectingPage>\n<svg id=connectingPageSvg viewBox=\"0 0 100 100\"><path fill=black d=\"M73,50c0-12.7-10.3-23-23-23S27,37.3,27,50 M30.9,50c0-10.5,8.5-19.1,19.1-19.1S69.1,39.5,69.1,50\"><animateTransform attributeName=transform type=rotate dur=1s from=\"0 50 50\" to=\"360 50 50\" repeatCount=\"indefinite\"/></path></svg>\n<div id=connectingPageText>Connecting...</div>\n<div id=connectingPageButton onclick={startWebSocket();}>Reconnect</div>\n</div>\n<div id=navBar>\n<svg id=controller onclick='{if(controller.length>1)document.getElementById(\"select\").style.display=(document.getElementById(\"select\").style.display==\"none\")?\"block\":\"none\";}' viewBox=\"0 0 100 68.5\">\n<path fill=#324D5B d=\"M11,14.5A4,8.5,19,0,0,29,54Q30.5,52,34,52H66Q69.5,52,71,54A4,8.5,-19,0,0,89,14.5L80,5H20Z\"/>\n<path fill=#1D323D d=\"M11,14.5L12.5,11Q13,10,15,9Q15,3,30,1C37,0,37,4,40,4H60C63,4,63,0,70,1Q85,3,85,9Q87,10,87.5,11L89,14.5Q72,5,66,10Q60,18,56,18H44Q40,18,34,10Q28,5,11,14.5\"/>\n<circle cx=50 cy=11.5 r=3 fill=\"#B8C4CB\"/>\n<circle cx=73.8 cy=17.3 r=2.7 fill=\"#597583\"/>\n<circle cx=73.8 cy=29.8 r=2.7 fill=\"#597583\"/>\n<circle cx=67.6 cy=23.55 r=2.7 fill=\"#597583\"/>\n<circle cx=80.1 cy=23.55 r=2.7 fill=\"#597583\"/>\n<circle cx=25 cy=23.18 r=7.7 fill=\"#2D4552\"/>\n<circle cx=25 cy=23.18 r=5.45 fill=\"#4F6B79\"/>\n<circle cx=25 cy=23.18 r=3.1 fill=\"#3F5966\"/>\n<circle cx=59.4 cy=37.1 r=7.7 fill=\"#2D4552\"/>\n<circle cx=59.4 cy=37.1 r=5.45 fill=\"#4F6B79\"/>\n<circle cx=59.4 cy=37.1 r=3.1 fill=\"#3F5966\"/>\n<rect x=32.8 y=34.9 rx=1.2 ry=1.2 width=12.5 height=4.8 fill=#4F6B79 />\n<rect x=36.7 y=31.1 rx=1.2 ry=1.2 width=4.8 height=12.5 fill=#4F6B79 />\n</svg>\n<div id=select style=display:none></div>\n<svg id='fullscreen'onclick=document.documentElement.webkitRequestFullScreen() viewBox='0 0 1 1'><path d=M0,0H0.35V0.15H0.15V0.35H0Z></path><path d=M1,0H0.65V0.15H0.85V0.35H1Z></path><path d=M1,1H0.65V0.85H0.85V0.65H1Z></path><path d=M0,1H0.35V0.85H0.15V0.65H0Z></path></svg>\n</div>\n<script>document.onwebkitfullscreenchange=function()\n{var fullscreen=document.getElementById('fullscreen');if(document.webkitIsFullScreen){fullscreen.classList='unfullscreen';fullscreen.onclick=function(){document.webkitExitFullscreen();};}\nelse{fullscreen.classList='';fullscreen.onclick=function(){document.documentElement.webkitRequestFullScreen();};}}\nfunction run(code,element,message,isOwner){eval(code);}\nvar webSocket;var controller=[];var selectedController=0;function webSocketMessage(a)\n{var parameter=a.data.split(',');var type=parseInt(parameter[0]);if(type==0)\n{controller=[];var oldControllerPage=document.getElementsByClassName('controllerPage');while(oldControllerPage.length>0)oldControllerPage[0].remove();document.getElementById('select').innerHTML='';document.getElementById(\"select\").style.display=\"none\";var controllerCount=parseInt(parameter[1]);selectedController=0;for(var i=0;i<controllerCount;i++)\n{controller[i]={element:[],div:document.createElement('div'),divOverlay:document.createElement('div'),divOverlayButton:document.createElement('div'),selectOption:document.createElement('div')};controller[i].div.classList='controllerPage';if(selectedController!=i)controller[i].div.style.display='none';document.body.append(controller[i].div);controller[i].divOverlay.classList='controllerPageOverlay';controller[i].divOverlay.style.display='flex';controller[i].div.append(controller[i].divOverlay);controller[i].divOverlayButton.classList='controllerPageOverlayButton';controller[i].divOverlayButton.innerText='Take Controller';controller[i].divOverlayButton.setAttribute('status','free');const buttonIndex=i;controller[i].divOverlayButton.onclick=function()\n{webSocket.send('1,'+buttonIndex);};controller[i].divOverlay.append(controller[i].divOverlayButton);if(parameter.length>(2+i))controller[i].selectOption.innerText=parameter[2+i];else controller[i].selectOption.innerText='Controller '+(i+1);if(i==selectedController)controller[i].selectOption.setAttribute('selected','true');else controller[i].selectOption.setAttribute('selected','false');controller[i].selectOption.setAttribute('status','free');controller[i].selectOption.onclick=function()\n{for(var c=0;c<controller.length;c++)\n{if(controller[c].selectOption!=this)\n{controller[c].selectOption.setAttribute('selected','false');controller[c].div.style.display='none';}\nelse\n{selectedController=c;controller[c].selectOption.setAttribute('selected','true');controller[c].div.style.display='block';}}};document.getElementById('select').append(controller[i].selectOption);}}\nelse if(type==1||type==2||type==3)\n{var controllerIndex=parseInt(parameter[1]);var elementIndex=parseInt(parameter[2]);var elementObject=controller[controllerIndex].element[elementIndex];var message=a.data.substring(a.data.indexOf(',',a.data.indexOf(',',a.data.indexOf(',')+1)+1)+1);if(type==1&&elementObject!=undefined)run(elementObject.update,elementObject.div,message,controller[controllerIndex].selectOption.getAttribute('status')=='mine');if(type==2&&elementObject!=undefined)\n{elementObject.div.remove();elementObject=undefined;}\nif(elementObject==undefined)\n{elementObject=controller[controllerIndex].element[elementIndex]={init:'',div:document.createElement('div'),update:''};elementObject.div.sendData=(data)=>webSocket.send('0,'+controllerIndex+','+elementIndex+','+data);controller[controllerIndex].div.append(elementObject.div);}\nif(type==2)run(elementObject.init=message,elementObject.div);if(type==3)elementObject.update=message;}\nelse if(type==4||type==5||type==6)\n{var controllerIndex=parseInt(parameter[1]);if(type==4)controller[controllerIndex].selectOption.setAttribute('status','free');if(type==5)controller[controllerIndex].selectOption.setAttribute('status','other');if(type==6)controller[controllerIndex].selectOption.setAttribute('status','mine');if(type==4)controller[controllerIndex].divOverlay.firstElementChild.setAttribute('status','free');if(type==5)controller[controllerIndex].divOverlay.firstElementChild.setAttribute('status','other');if(type==6)controller[controllerIndex].divOverlay.firstElementChild.setAttribute('status','mine');if(type==4||type==5)controller[controllerIndex].divOverlay.style.display='flex';if(type==6)controller[controllerIndex].divOverlay.style.display='none';}}";
char* Controller::webPage1 = "function startWebSocket()\n{document.getElementById('connectingPageSvg').style.display='block';document.getElementById('connectingPageText').style.display='block';document.getElementById('connectingPageButton').style.display='none';webSocket=new WebSocket('ws://'+window.location.hostname+':";
char* Controller::webPage2 = "/');webSocket.onmessage=webSocketMessage;webSocket.onopen=function()\n{document.getElementById('connectingPage').style.display='none';}\nwebSocket.onclose=function()\n{document.getElementById('connectingPage').style.display='flex';document.getElementById('connectingPageSvg').style.display='none';document.getElementById('connectingPageText').style.display='none';document.getElementById('connectingPageButton').style.display='block';}}\nstartWebSocket();</script>\n</body>\n</html>";

Controller::Controller(int count, int setPort) : webSocket(setPort)
{
	port = setPort;

	webSocket.onEvent([=](uint8_t num, WStype_t type, uint8_t * payload, size_t length){ webSocketEvent(num, type, payload, length); });
	webSocket.begin();

	for(int i = 0; i < count; i++)
	{
		singleController.push_back(new SingleController(i, this));
		singleControllerName.push_back(FastString::add({ (char*)"Controller ", (char*)String(i + 1).c_str() }));
	}
}

Controller::Controller(std::initializer_list<char*> nameList, int setPort) : webSocket(setPort)
{
	port = setPort;

	webSocket.onEvent([=](uint8_t num, WStype_t type, uint8_t * payload, size_t length){ webSocketEvent(num, type, payload, length); });
	webSocket.begin();

	std::vector<char*> nameVector(nameList);

	for(int i = 0; i < nameVector.size(); i++)
	{
		singleController.push_back(new SingleController(i, this));
		if(nameVector[i] == "") singleControllerName.push_back(FastString::add({ (char*)"Controller ", (char*)String(i + 1).c_str() }));
		else singleControllerName.push_back(nameVector[i]);
	}
}

const char* Controller::getWebPage()
{
	return webPage;
}

void Controller::init()
{
	std::vector<char*> webPageVector;

	webPageVector.push_back(webPage0);

	webPageVector.push_back((char*)"webSocketMessage({ data: `");
	webPageVector.push_back((char*)"0,");
	String controllerCountStr = String(singleController.size());
	webPageVector.push_back((char*)controllerCountStr.c_str());
	for(int c = 0; c < singleController.size(); c++)
	{
		webPageVector.push_back((char*)",");
		webPageVector.push_back(singleControllerName[c]);
	}
	webPageVector.push_back((char*)"` });");

	int maxNumberStr = singleController.size();
	for(int c = 0; c < singleController.size(); c++)
	{
		if(singleController[c]->element.size() > maxNumberStr) maxNumberStr = singleController[c]->element.size();
	}

	String numberStr[maxNumberStr];
	for(int i = 0; i < maxNumberStr; i++) numberStr[i] = String(i);
	

	for(int c = 0; c < singleController.size(); c++)
	{
		/*if(singleController[c]->hasOwner == true)
		{
			webPageVector.push_back((char*)"webSocketMessage({ data: `");
			webPageVector.push_back((char*)"5,");
			webPageVector.push_back((char*)numberStr[c].c_str());
			webPageVector.push_back((char*)"` });");
		}
		else
		{
			webPageVector.push_back((char*)"webSocketMessage({ data: `");
			webPageVector.push_back((char*)"4,");
			webPageVector.push_back((char*)numberStr[c].c_str());
			webPageVector.push_back((char*)"` });");
		}*/

		for(int e = 0; e < singleController[c]->element.size(); e++)
		{
			webPageVector.push_back((char*)"webSocketMessage({ data: `");
			webPageVector.push_back((char*)"2,");
			webPageVector.push_back((char*)numberStr[c].c_str());
			webPageVector.push_back((char*)",");
			webPageVector.push_back((char*)numberStr[e].c_str());
			webPageVector.push_back((char*)",");
			webPageVector.insert(webPageVector.end(), singleController[c]->element[e]->initJs.begin(), singleController[c]->element[e]->initJs.end());
			webPageVector.push_back((char*)"` });");

			webPageVector.push_back((char*)"webSocketMessage({ data: `");
			webPageVector.push_back((char*)"3,");
			webPageVector.push_back((char*)numberStr[c].c_str());
			webPageVector.push_back((char*)",");
			webPageVector.push_back((char*)numberStr[e].c_str());
			webPageVector.push_back((char*)",");
			webPageVector.insert(webPageVector.end(), singleController[c]->element[e]->updateJs.begin(), singleController[c]->element[e]->updateJs.end());
			webPageVector.push_back((char*)"` });");
		}
	}

	webPageVector.push_back(webPage1);
	String portStr = String(port);
	webPageVector.push_back((char*)portStr.c_str());

	webPageVector.push_back(webPage2);

	int size = 0;
	for(int i = 0; i < webPageVector.size(); i++) size += strlen(webPageVector[i]);
	webPage = (char*)malloc(size + 1);
	int charPosition = 0;
	for(int i = 0; i < webPageVector.size(); i++)
	{
		for(int c = 0; webPageVector[i][c] != '\0'; c++)
		{
			webPage[charPosition] = webPageVector[i][c];
			charPosition++;
		}
	}
	webPage[charPosition] = '\0';
}

void Controller::update()
{
	webSocket.loop();
}

SingleController& Controller::operator[] (int index)
{
	return *singleController[index];
}

#include "user_interface.h"

void Controller::webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
	//Serial.print("free memeory: ");
	//Serial.println(system_get_free_heap_size());

	/*Server-Sent
	0 - controllerCount (0 , count , [controllerName ,])
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
		/*std::vector<char*> controllerMessage;
		controllerMessage.push_back((char*)"0,");
		controllerMessage.push_back((char*)String(singleController.size()).c_str());
		for(int c = 0; c < singleController.size(); c++)
		{
			controllerMessage.push_back((char*)",");
			controllerMessage.push_back(singleControllerName[c]);
		}
		char* stringA = FastString::add(controllerMessage);
		webSocket.sendTXT(num, stringA);
		free(stringA);*/

		for(int c = 0; c < singleController.size(); c++)
		{
			if(singleController[c]->hasOwner == true)
			{
				char* stringB = FastString::add({ (char*)"5,", (char*)String(c).c_str() });
				webSocket.sendTXT(num, stringB);
				free(stringB);
			}
			else
			{
				char* stringB = FastString::add({ (char*)"4,", (char*)String(c).c_str() });
				webSocket.sendTXT(num, stringB);
				free(stringB);
			}

			for(int e = 0; e < singleController[c]->element.size(); e++)
			{
				/*char* stringB = FastString::add({ (char*)"2,", (char*)String(c).c_str(), (char*)",", (char*)String(e).c_str(), (char*)",", singleController[c]->element[e]->initJs });
				char* stringC = FastString::add({ (char*)"3,", (char*)String(c).c_str(), (char*)",", (char*)String(e).c_str(), (char*)",", singleController[c]->element[e]->updateJs });
				webSocket.sendTXT(num, stringB);
				webSocket.sendTXT(num, stringC);
				free(stringB);
				free(stringC);*/
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

				char* stringA = FastString::add({ (char*)"4,", (char*)String(c).c_str() });
				webSocket.broadcastTXT(stringA);
				free(stringA);
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
				singleController[controllerIndex]->element[elementIndex]->data(num, nextNumber);
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

			char* stringA = FastString::add({ (char*)"5,", (char*)String(controllerIndex).c_str() });
			char* stringB = FastString::add({ (char*)"6,", (char*)String(controllerIndex).c_str() });
			webSocket.broadcastTXT(stringA);
			webSocket.sendTXT(num, stringB);
			free(stringA);
			free(stringB);
		}

		free(message);
	}
}
