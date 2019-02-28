#include "Controller.h"

char * Controller::webPageOnMessage = "if(message.data.charCodeAt(0)==1){selectedPage=parseInt(message.data.substring(1));element=pageList[selectedPage].div;code='';}\
else if(message.data.charCodeAt(0)==2){\
var index=parseInt(message.data.substring(1));\
if(element.childrenList[index]==undefined){\
var newElement=element.childrenList[index]=document.createElement('div');\
element.container.append(newElement);\
newElement.childrenList=[];\
newElement.container=newElement;\
newElement.parent=element;\
newElement.send=function(message=''){webSocket.send(this.parent.getSelector(this)+message);};\
newElement.getSelector=function(selectElememt){return this.parent.getSelector(this)+'\001'+this.childrenList.findIndex(e=>e==selectElememt);};\
}element=element.childrenList[index];code='';}\
else if(message.data.charCodeAt(0) == 3){eval(code);code='';}\
else code += message.data;";

char * Controller::webPageOnOpen = "document.body.insertAdjacentHTML('beforeend', \"\
<svg id='controller'stroke-linecap='round'viewBox='0 0 100 68.5'style='position:absolute;top:0px;left:0px;height:80px;margin:10px;cursor:pointer;z-index:900;'>\
<path fill='#324D5B'd='M11,14.5A4,8.5,19,0,0,29,54Q30.5,52,34,52H66Q69.5,52,71,54A4,8.5,-19,0,0,89,14.5L80,5H20Z'/><path stroke='#597583'stroke-width='5.2'd='M73.8,17.3Z M73.8,29.8Z M67.6,23.5Z M80.1,23.5Z'/>\
<path fill='#1D323D'd='M11,14.5L12.5,11Q13,10,15,9Q15,3,30,1C37,0,37,4,40,4H60C63,4,63,0,70,1Q85,3,85,9Q87,10,87.5,11L89,14.5Q72,5,66,10Q60,18,56,18H44Q40,18,34,10Q28,5,11,14.5'/>\
<path stroke='#2D4552'stroke-width='15.4'd='M25,23.1Z M59.4,37.1Z'/><path stroke='#4F6B79'stroke-width='10.8'd='M25,23.1Z M59.4,37.1Z'/><path stroke='#3F5966'stroke-width='6.2'd='M25,23.1Z M59.4,37.1Z'/>\
<rect x='32.8'y='34.9'rx='1.2'ry='1.2'width='12.5'height='4.8'fill='#4F6B79'/><rect x='36.7'y='31.1'rx='1.2'ry='1.2'width='4.8'height='12.5'fill='#4F6B79'/><circle cx='50'cy='11.5'r='3'fill='#B8C4CB'/></svg>\
\
<div id='overlay'class='fullPage'style='z-index:500;display:flex;justify-content:center;align-items:center;background:rgba(0,0,0,0.25);'>\
<div id='overlayButton'class='noSelect'style='color:dimgrey;background:whitesmoke;font:bold 25px sans-serif;padding:20px;border-left:10px solid steelblue;cursor:pointer;'>Take Controller</div></div>\
<div id='menuOverlay'class='fullPage'style='z-index:800;display:none;'></div>\
<div id='menu'style='position:absolute;top:0px;left:-100vw;max-width:calc(100vw - 100px);height:calc(100vh - 55px);padding-top:55px;background:whitesmoke;transition:left 1s;z-index: 1000;overflow:auto;'>\
<svg id='menuClose'viewbox='0 0 100 100'style='stroke:#000;stroke-width:25;position:absolute;right:0;top:0;width:25px;height:25px;padding:15px;cursor:pointer;'><line x1='10'y1='10'x2='90'y2='90'/><line x1='90'y1='10'x2='10'y2='90'/></svg>\
<div id='menuSelector'style='background:steelblue;width:10px;position:absolute;left:10px;transition:top 0.25s ease 0s,height  0.25s ease 0s;'></div></div>\
\
<style>#controller{position:absolute;top:0px;left:0px;height:80px;margin:10px;cursor:pointer;z-index:900;}\
.menuButton{padding:20px;font:bold 25px sans-serif;cursor:pointer;overflow-wrap:break-word;border-top:1px solid grey;margin:0 20px;}\
.fullPage{position:absolute;top:0;left:0;width:100vw;height:100vh;}.noSelect{-webkit-touch-callout:none;-webkit-user-select:none;-khtml-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none;}</style>\");\
\
var page,menuButton;\
\
document.getElementById('overlayButton').addEventListener('click',function(){pageList[selectedPage].div.send();});\
document.getElementById('controller').addEventListener('click',function(){if(pageList.length>1){document.getElementById('menu').style.left='0';document.getElementById('menuOverlay').style.display='block';}});\
document.getElementById('menuClose').addEventListener('click',function(){document.getElementById('menu').style.left=-document.getElementById('menu').offsetWidth+'px';document.getElementById('menuOverlay').style.display='none';});\
document.getElementById('menuOverlay').addEventListener('click',function(){document.getElementById('menu').style.left=-document.getElementById('menu').offsetWidth+'px';document.getElementById('menuOverlay').style.display='none';});\
\
for(var i=0;i<pageList.length;i++){\
document.body.append(page=pageList[i].div=document.createElement('div'));\
page.container=page;\
page.childrenList=[];\
page.classList='fullPage';\
page.style='z-index:0;display:none;';\
page.index=i;\
page.setOwner=function(owner){\
pageList[selectedPage].hasOwner=(owner!=0);\
pageList[selectedPage].isOwner=(owner==1);\
if(owner==0)pageList[this.index].menuButton.style.color=document.getElementById('overlayButton').style.color='dimgrey';\
if(owner==1)pageList[this.index].menuButton.style.color=document.getElementById('overlayButton').style.color='darkgreen';\
if(owner==2)pageList[this.index].menuButton.style.color=document.getElementById('overlayButton').style.color='darkred';\
document.getElementById('overlay').style.display=(owner==1&&this.index==selectedPage)?'none':'flex';};\
page.send=function(message=''){webSocket.send('\001'+this.index+message);};\
page.getSelector=function(selectElememt){return'\001'+this.index+'\001'+this.childrenList.findIndex(e=>e==selectElememt);};\
\
document.getElementById('menu').append(menuButton=pageList[i].menuButton=document.createElement('div'));\
menuButton.innerText=pageList[i].name;\
menuButton.classList='menuButton noSelect';\
menuButton.style='color:dimgrey';\
menuButton.index=i;\
menuButton.addEventListener('click', function(){\
selectedPage=this.index;\
for(var i=0;i<pageList.length;i++)pageList[i].div.style.display=i==selectedPage?'block':'none';\
var menuSelector=document.getElementById('menuSelector');\
menuSelector.style.top=this.offsetTop+'px';\
menuSelector.style.height=(this.offsetHeight+1)+'px';\
document.getElementById('overlayButton').style.color=this.style.color;\
document.getElementById('overlay').style.display=(pageList[selectedPage].hasOwner&&pageList[selectedPage].isOwner)?'none':'flex';\
});}\
pageList[selectedPage].menuButton.dispatchEvent(new CustomEvent('click'));\
window.addEventListener('resize',function(){pageList[selectedPage].menuButton.dispatchEvent(new CustomEvent('click'));});";

char * Controller::webPageOnClose = "document.body.insertAdjacentHTML('beforeend', \"<style>body{display:flex;justify-content:center;align-items:center;height:100vh;overflow:hidden;flex-direction:column;}</style>\
<div style='font:bold 50px monospace;margin:10px;color:green;padding:10px 20px;border:5px solid green;\
-webkit-touch-callout:none;-webkit-user-select:none;-khtml-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none;' onclick='startWebSocket()'>Reconnect</div>\");";

char * Controller::webPageLoading = "document.body.insertAdjacentHTML('beforeend', \"<style>body{display:flex;justify-content:center;align-items:center;height:100vh;overflow:hidden;flex-direction:column;}@keyframes dash{0%,25%{stroke-dashoffset:199.99;}100%{stroke-dashoffset:0;}}</style>\
<svg viewBox='0 0 100 100' style='width:100px;height:100px;stroke-dasharray:200;animation:dash 2s linear alternate infinite;'>\
<path class='path' d='M90 50A40 40 0 1 1 50 10' fill='none' stroke='green'stroke-width='10' stroke-linecap='round'></path>\
<animateTransform attributeName='transform' type='rotate' dur='1s' from='0' to='360' repeatCount='indefinite'></svg><div style='font:bold 50px monospace;margin:10px;color:green;'>Connecting</div>\");";

char * Controller::fullscreenToggle = "document.body.insertAdjacentHTML('beforeend', \"<svg id='fullscreen'onclick='if(document.webkitIsFullScreen)document.webkitExitFullscreen();else document.documentElement.webkitRequestFullScreen();' viewBox='0 0 1 1'>\
<path d='M0,0H0.35V0.15H0.15V0.35H0Z'></path><path d='M1,0H0.65V0.15H0.85V0.35H1Z'></path><path d='M1,1H0.65V0.85H0.85V0.65H1Z'></path><path d='M0,1H0.35V0.85H0.15V0.65H0Z'></path></svg>\
<style>#fullscreen{position:absolute;top:0;right:0;width:80px;height:80px;margin:10px;cursor:pointer;z-index:1100;}\
#fullscreen path:nth-child(1){transform-origin:17.5% 17.5%;transition:0.5s ease;}html:fullscreen #fullscreen path:nth-child(1){transform:rotate3D(1,-1,0,180deg);}\
#fullscreen path:nth-child(2){transform-origin:82.5% 17.5%;transition:0.5s ease;}html:fullscreen #fullscreen path:nth-child(2){transform:rotate3D(1,1,0,180deg);}\
#fullscreen path:nth-child(3){transform-origin:82.5% 82.5%;transition:0.5s ease;}html:fullscreen #fullscreen path:nth-child(3){transform:rotate3D(1,-1,0,180deg);}\
#fullscreen path:nth-child(4){transform-origin:17.5% 82.5%;transition:0.5s ease;}html:fullscreen #fullscreen path:nth-child(4){transform:rotate3D(1,1,0,180deg);}</style>\");";

Controller::Controller(int count, int setPort) : webSocket(setPort)
{
	port = setPort;

	webSocket.onEvent([=](uint8_t num, WStype_t type, uint8_t * payload, size_t length){ webSocketEvent(num, type, payload, length); });
	webSocket.begin();

	for(int i = 0; i < count; i++)
	{
		singleController.push_back(new SingleController(i, this));
		//singleControllerName.push_back(FastString::add({ (char*)"Controller ", (char*)String(i + 1).c_str() }));
		size_t nameSize = snprintf(NULL, 0, "Controller %i", i + 1) + 1;
    	char * buffer = (char*)malloc(nameSize);
    	sprintf(buffer, "Controller %i", i + 1);
		singleControllerName.push_back(buffer);
	}

	createWebPage();
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
		if(nameVector[i] == "")
		{
			//singleControllerName.push_back(FastString::add({ (char*)"Controller ", (char*)String(i + 1).c_str() }));
			size_t nameSize = snprintf(NULL, 0, "Controller %i", i + 1) + 1;
			char * buffer = (char*)malloc(nameSize);
			sprintf(buffer, "Controller %i", i + 1);
			singleControllerName.push_back(buffer);
		}
		else singleControllerName.push_back(nameVector[i]);
	}

	createWebPage();
}

char* Controller::getWebPage()
{
	return webPage;
}

void Controller::createWebPage()
{
	std::vector<char*> webPageVector;

	webPageVector.push_back("<!DOCTYPE html><html><head><title>Controller</title><meta name='viewport'content='width=device-width,initial-scale=1.0,user-scalable=no'><script>");

	webPageVector.push_back("var pageList=[");

	for(int i = 0; i < singleController.size(); i++)
	{
		webPageVector.push_back("{name:'");
		webPageVector.push_back(singleControllerName[i]);
		webPageVector.push_back("',div:undefined,hasOwner:false,isOwner:false,menuButton:undefined},");
	}

	webPageVector.push_back("];");

	webPageVector.push_back("var webSocket,selectedPage=0,element,code='';function startWebSocket(){webSocket=new WebSocket('ws://'+window.location.hostname +':");

	String portStr = String(port);
	webPageVector.push_back((char*)portStr.c_str());

	webPageVector.push_back("/');webSocket.onmessage=function(message){");

	webPageVector.push_back(webPageOnMessage); //onMessage

	webPageVector.push_back("};webSocket.onopen = function(){while(document.body.firstChild) document.body.removeChild(document.body.firstChild);");

	webPageVector.push_back(webPageOnOpen); //onOpen
	webPageVector.push_back(fullscreenToggle);

	webPageVector.push_back("};webSocket.onclose=webSocket.onerror=function(){while(document.body.firstChild) document.body.removeChild(document.body.firstChild);");

	webPageVector.push_back(webPageOnClose); //onClose & onError
	webPageVector.push_back(fullscreenToggle);

	webPageVector.push_back("};while(document.body.firstChild) document.body.removeChild(document.body.firstChild);");

	webPageVector.push_back(webPageLoading); //loading
	webPageVector.push_back(fullscreenToggle);

	webPageVector.push_back("}</script></head><body onload='startWebSocket()'></body></html>");

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
	1 - select controller page (\0, controllerIndex)
	2 - select child (\1, childIndex)
	3 - run js code (\2)
	defaul - code snippet (codeSnippet)
	*/

	if (type == WStype_CONNECTED)
	{
		for(int c = 0; c < singleController.size(); c++) singleController[c]->connectedEvent(num);
	}
	else if (type == WStype_DISCONNECTED)
	{
		for(int c = 0; c < singleController.size(); c++) singleController[c]->disconnectedEvent(num);
	}
	else if (type == WStype_TEXT)
	{
		if(length == 0) return;
		char *message = (char*)malloc(length + 1);
		for (int i = 0; i < length; i++) message[i] = payload[i];
		message[length] = '\0';

		if(message[0] == 1)
		{
			int childIndex = strtol(++message, &message, 10);
			if(singleController.size() <= childIndex) return;
			singleController[childIndex]->parseData(num, message);
		}
		else
		{
			//Serial.println("Controller recived message!!!");
		}

		free(message);
	}
}
