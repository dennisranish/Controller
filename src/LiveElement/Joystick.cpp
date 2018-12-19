#include "Joystick.h"

char* Joystick::jsInitCode = R"(
element.style = 'border: 2px solid black; display: flex; justify-content: center; align-items: center; padding: 7px;';
element.innerWidth = parseInt(element.style.width, 10) / 2;
element.innerHeight = parseInt(element.style.height, 10) / 2;
element.dot = document.createElement('div');element.append(element.dot);
element.title = document.createElement('div');element.append(element.title);
element.labelA = document.createElement('div');element.append(element.labelA);
element.labelB = document.createElement('div');element.append(element.labelB);
element.labelC = document.createElement('div');element.append(element.labelC);
element.labelD = document.createElement('div');element.append(element.labelD);
element.title.style = 'color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';
element.labelA.style = 'position: fixed; transform: translate(0px, -' + (element.innerHeight / 2) + 'px); color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';
element.labelB.style = 'position: fixed; transform: translate(' + (element.innerWidth / 2) + 'px, 0px); color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';
element.labelC.style = 'position: fixed; transform: translate(0px, ' + (element.innerHeight / 2) + 'px); color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';
element.labelD.style = 'position: fixed; transform: translate(-' + (element.innerWidth / 2) + 'px, 0px); color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';
element.touchId = -1;
if(element.innerWidth > 0 || element.innerHeight > 0) element.valueScale = 100 / Math.max(this.innerWidth / 2, this.innerHeight / 2);
else element.valueScale = 0;
element.addEventListener('touchstart', touchEvent, false);
element.addEventListener('touchmove', touchEvent, false);
element.addEventListener('touchcancel', touchEvent, false);
element.addEventListener('touchend', touchEvent, false);
function touchEvent(event)
{
	if(this.touchId == -1) this.touchId = 0;
	var index = -1;
	for(var i = 0; i < event.touches.length; i++) if(event.touches[i].identifier == this.touchId) index = i;
	if(index != -1)
	{
		this.x = event.touches[index].pageX - (this.getBoundingClientRect().x + (this.getBoundingClientRect().width / 2));
		this.y = event.touches[index].pageY - (this.getBoundingClientRect().y + (this.getBoundingClientRect().height / 2));
		if(this.x > this.innerWidth / 2) this.x = this.innerWidth / 2;
		if(this.x < -this.innerWidth / 2) this.x = -this.innerWidth / 2;
		if(this.y > this.innerHeight / 2) this.y = this.innerHeight / 2;
		if(this.y < -this.innerHeight / 2) this.y = -this.innerHeight / 2;
		this.x *= this.valueScale;
		this.y *= this.valueScale;
	}
	else
	{
		this.touchId = -1;
		this.x = 0;
		this.y = 0;
	}
	this.dot.style.top = this.x + 'px';
	this.dot.style.left = this.y + 'px';
	this.sendData(this.x + ',' + this.y);
})";

char* Joystick::jsUpdateCode = R"(
if(message.charCodeAt(0) == 2)
{
	element.title.innerText = message.substring(1);
}
else if(message.charCodeAt(0) == 3)
{
	var label = message.substring(1).split(',');

	element.labelA.innerText = label[0];
	element.labelB.innerText = label[1];
	element.labelC.innerText = label[2];
	element.labelD.innerText = label[3];
}
else if(!isOwner)
{
	var position = message.split(',');

	element.x = position[0] * element.valueScale;
	element.y = position[1] * element.valueScale;
	if(element.x > element.innerWidth / 2) element.x = element.innerWidth / 2;
	if(element.x < -element.innerWidth / 2) element.x = -element.innerWidth / 2;
	if(element.y > element.innerHeight / 2) element.y = element.innerHeight / 2;
	if(element.y < -element.innerHeight / 2) element.y = -element.innerHeight / 2;

	element.dot.style.top = element.x + 'px';
	element.dot.style.left = element.y + 'px';
})";

Joystick::Joystick(char* name)
{
	char* stringA = FastString::add({ (char*)"element.style = 'width: 150px; height: 150px;';", jsInitCode });
	setInitJs(stringA);
	setUpdateJs(jsUpdateCode);
	setName(name);
	free(stringA);
}

Joystick::Joystick(char* name, char* style)
{
	char* stringA = FastString::add({ (char*)"element.style = '", style, (char*)"';", jsInitCode });
	setInitJs(stringA);
	setUpdateJs(jsUpdateCode);
	setName(name);
	free(stringA);
}

void Joystick::setTitle(char* newTitle)
{
	title = newTitle;

	char* stringA = FastString::add({ (char*)"\x002", title });
	broadcastData(stringA);
	free(stringA);
}

void Joystick::setLabels(char* newTopLabel, char* newRightLabel, char* newBottomLabel, char* newLeftLabel)
{
	topLabel = newTopLabel;
	rightLabel = newRightLabel;
	bottomLabel = newBottomLabel;
	leftLabel = newLeftLabel;

	char* stringA = FastString::add({ (char*)"\x003", topLabel, (char*)",", rightLabel, (char*)",", bottomLabel, (char*)",", leftLabel });
	broadcastData(stringA);
	free(stringA);
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

void Joystick::connected(uint8_t num)
{
	char* stringA = FastString::add({ (char*)"\x002", title });
	char* stringB = FastString::add({ (char*)"\x003", topLabel, (char*)",", rightLabel, (char*)",", bottomLabel, (char*)",", leftLabel });
	sendData(num, stringA);
	sendData(num, stringB);
	free(stringA);
	free(stringB);
}

void Joystick::data(uint8_t num, char* data)
{
	String dataS = String(data);
	xValue = dataS.toFloat();
	yValue = dataS.substring(dataS.indexOf(',') + 1).toFloat();

	char* stringA = FastString::add({ (char*)String(xValue).c_str(), (char*)",", (char*)String(yValue).c_str() });
	broadcastData(stringA);
	free(stringA);

	if(updateCallback != NULL) updateCallback(xValue, yValue);
}

void Joystick::disconnected(uint8_t num)
{

}