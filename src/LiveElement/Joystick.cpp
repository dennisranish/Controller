#include "Joystick.h"

char * Joystick::jsCode = R"(
element.style.cssText += 'border: 2px solid black; display: flex; justify-content: center; align-items: center; padding: 7px;';
element.innerWidth = parseInt(element.style.width, 10);
element.innerHeight = parseInt(element.style.height, 10);
element.titleLabel = document.createElement('div');element.append(element.titleLabel);
element.labelA = document.createElement('div');element.append(element.labelA);
element.labelB = document.createElement('div');element.append(element.labelB);
element.labelC = document.createElement('div');element.append(element.labelC);
element.labelD = document.createElement('div');element.append(element.labelD);
element.dot = document.createElement('div');element.append(element.dot);
element.dot.style = 'width:10px;height:10px;border:2px solid black;position:relative;z-index:101;transform: translate(7px)';
element.dotEcho = document.createElement('div');element.append(element.dotEcho);
element.dotEcho.style = 'width:10px;height:10px;border:2px solid steelBlue;position:relative;z-index:100;transform: translate(-7px)';
element.titleLabel.style = 'color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;position: absolute;top: 0;left: 0;padding-left: 4px;';
element.labelA.style = 'position: fixed; transform: translate(0px, -' + (element.innerHeight / 2) + 'px); color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';
element.labelB.style = 'position: fixed; transform: translate(' + (element.innerWidth / 2) + 'px, 0px); color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';
element.labelC.style = 'position: fixed; transform: translate(0px, ' + (element.innerHeight / 2) + 'px); color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';
element.labelD.style = 'position: fixed; transform: translate(-' + (element.innerWidth / 2) + 'px, 0px); color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';
element.touchId = -1;
if(element.innerWidth > 0 || element.innerHeight > 0) element.valueScale = 100 / Math.max(element.innerWidth / 2, element.innerHeight / 2);
else element.valueScale = 0;
element.x=0;element.y=0;element.recivedX=0;element.recivedY=0;
let savedElement = element;
element.addEventListener('touchstart', touchEvent, false);
element.addEventListener('touchmove', touchEvent, false);
element.addEventListener('touchcancel', touchEvent, false);
element.addEventListener('touchend', touchEvent, false);
function touchEvent(event)
{
	var index = -1;
	if(this.touchId == -1)
	{
		for(var i = 0; i < event.touches.length; i++)
		{
			if(event.touches[i].target == this) { index = i; this.touchId = event.touches[i].identifier; break; }
		}
	}
	else
	{
		for(var i = 0; i < event.touches.length; i++)
		{
			if(event.touches[i].identifier == this.touchId) { index = i; break; }
		}
	}
	if(index != -1)
	{
		this.x = event.touches[index].pageX - (this.getBoundingClientRect().x + (this.getBoundingClientRect().width / 2));
		this.y = event.touches[index].pageY - (this.getBoundingClientRect().y + (this.getBoundingClientRect().height / 2));
		if(this.x > this.innerWidth / 2) this.x = this.innerWidth / 2;
		if(this.x < -this.innerWidth / 2) this.x = -this.innerWidth / 2;
		if(this.y > this.innerHeight / 2) this.y = this.innerHeight / 2;
		if(this.y < -this.innerHeight / 2) this.y = -this.innerHeight / 2;
	}
	else
	{
		this.touchId = -1;
		this.x = 0;
		this.y = 0;
	}

	this.dot.style.top = this.y + 'px';
	this.dot.style.left = this.x + 'px';

	if(this.recived)
	{
		this.recived = false;
		this.send((this.x * this.valueScale) + ',' + (this.y * this.valueScale));
	}

	clearInterval(this.interval);
	
	this.interval = setInterval(function()
	{
		if(savedElement.recivedX != savedElement.x || savedElement.recivedY != savedElement.y)
		{
			savedElement.recived = false;
			savedElement.lastSent = new Date();
			savedElement.send((savedElement.x * savedElement.valueScale) + ',' + (savedElement.y * savedElement.valueScale));
		}
		else clearInterval(savedElement.interval);
	}, 250);
})";

Joystick::Joystick(char * name, char * style)
{
	this->name = name;
	this->style = style;
	defualDisplay = "flex";
}

void Joystick::setTitle(char * newTitle)
{
	title = newTitle;

	broadcastSelectSelf();
	broadcastData("element.titleLabel.innerText = '");
	broadcastData(title);
	broadcastData("';");
	broadcastRun();
}

void Joystick::setLabels(char * newTopLabel, char * newRightLabel, char * newBottomLabel, char * newLeftLabel)
{
	topLabel = newTopLabel;
	rightLabel = newRightLabel;
	bottomLabel = newBottomLabel;
	leftLabel = newLeftLabel;

	broadcastSelectSelf();
	broadcastData("element.labelA.innerText = '");broadcastData(topLabel);broadcastData("';");
	broadcastData("element.labelB.innerText = '");broadcastData(rightLabel);broadcastData("';");
	broadcastData("element.labelC.innerText = '");broadcastData(bottomLabel);broadcastData("';");
	broadcastData("element.labelD.innerText = '");broadcastData(leftLabel);broadcastData("';");
	broadcastRun();
}

void Joystick::setUpdateCallback(void (*newUpdateCallback)(double, double))
{
	updateCallback = newUpdateCallback;
}

double Joystick::x()
{
	return xValue;
}

double Joystick::y()
{
	return yValue;
}

void Joystick::connectedEvent(uint8_t num)
{
	String xString = String(xValue);
	String yString = String(yValue);

	selectSelf(num);
	sendData(num, "element.style = '");
	sendData(num, style);
	sendData(num, "';");
	sendData(num, jsCode);
	sendData(num, "element.titleLabel.innerText = '");sendData(num, title);sendData(num, "';");
	sendData(num, "element.labelA.innerText = '");sendData(num, topLabel);sendData(num, "';");
	sendData(num, "element.labelB.innerText = '");sendData(num, rightLabel);sendData(num, "';");
	sendData(num, "element.labelC.innerText = '");sendData(num, bottomLabel);sendData(num, "';");
	sendData(num, "element.labelD.innerText = '");sendData(num, leftLabel);sendData(num, "';");
	sendData(num, "element.recivedX=");
	sendData(num, (char*)xString.c_str());
	sendData(num, "/element.valueScale;element.recivedY=");
	sendData(num, (char*)yString.c_str());
	sendData(num, " /element.valueScale;\
	if(element.recivedX>element.innerWidth/2)element.recivedX=element.innerWidth/2;\
	if(element.recivedX<-element.innerWidth/2)element.recivedX=-element.innerWidth/2;\
	if(element.recivedY>element.innerHeight/2)element.recivedY=element.innerHeight/2;\
	if(element.recivedY<-element.innerHeight/2)element.recivedY=-element.innerHeight/2;\
	if(!pageList[selectedPage].isOwner){element.dot.style.left=element.recivedX+'px';element.dot.style.top=element.recivedY+'px';}\
	element.dotEcho.style.left=element.recivedX+'px';element.dotEcho.style.top=element.recivedY+'px';\
	element.recived=true;");
	sendRun(num);
}

void Joystick::dataEvent(uint8_t num, char* data)
{
	String dataS = String(data);
	xValue = dataS.toFloat();
	yValue = dataS.substring(dataS.indexOf(',') + 1).toFloat();

	String xString = String(xValue);
	String yString = String(yValue);

	broadcastSelectSelf();
	broadcastData("element.recivedX=");
	broadcastData((char*)xString.c_str());
	broadcastData("/element.valueScale;element.recivedY=");
	broadcastData((char*)yString.c_str());
	broadcastData(" /element.valueScale;\
	if(element.recivedX>element.innerWidth/2)element.recivedX=element.innerWidth/2;\
	if(element.recivedX<-element.innerWidth/2)element.recivedX=-element.innerWidth/2;\
	if(element.recivedY>element.innerHeight/2)element.recivedY=element.innerHeight/2;\
	if(element.recivedY<-element.innerHeight/2)element.recivedY=-element.innerHeight/2;\
	if(!pageList[selectedPage].isOwner){element.dot.style.left=element.recivedX+'px';element.dot.style.top=element.recivedY+'px';}\
	element.dotEcho.style.left=element.recivedX+'px';element.dotEcho.style.top=element.recivedY+'px';\
	element.recived=true;");
	broadcastRun();

	if(updateCallback != NULL) updateCallback(xValue, yValue);
}

void Joystick::disconnectedEvent(uint8_t num)
{
	if(parentSingleController->getHasOwner() && parentSingleController->getOwner() == num)
	{
		xValue = 0;
		yValue = 0;

		String xString = String(xValue);
		String yString = String(yValue);

		broadcastSelectSelf();
		broadcastData("element.recivedX=");
		broadcastData((char*)xString.c_str());
		broadcastData("/element.valueScale;element.recivedY=");
		broadcastData((char*)yString.c_str());
		broadcastData(" /element.valueScale;\
		if(element.recivedX>element.innerWidth/2)element.recivedX=element.innerWidth/2;\
		if(element.recivedX<-element.innerWidth/2)element.recivedX=-element.innerWidth/2;\
		if(element.recivedY>element.innerHeight/2)element.recivedY=element.innerHeight/2;\
		if(element.recivedY<-element.innerHeight/2)element.recivedY=-element.innerHeight/2;\
		if(!pageList[selectedPage].isOwner){element.dot.style.left=element.recivedX+'px';element.dot.style.top=element.recivedY+'px';}\
		element.dotEcho.style.left=element.recivedX+'px';element.dotEcho.style.top=element.recivedY+'px';\
		element.recived=true;");
		broadcastRun();

		if(updateCallback != NULL) updateCallback(xValue, yValue);
	}
}