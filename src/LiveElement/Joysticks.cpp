#include <Joystick.h>

Joystick(const String & name)
{
	setInitJs("\
\
element.style = 'width: 200px; height: 200px; border: 2px solid black; display: flex; justify-content: center; align-items: center; padding: 7px;';\
\
var dot = document.createElement('div');\
var LabelA = document.createElement('div');\
var LabelB = document.createElement('div');\
var LabelC = document.createElement('div');\
var LabelD = document.createElement('div');\
\
element.append(dot);\
element.append(LabelA);\
element.append(LabelB);\
element.append(LabelC);\
element.append(LabelD);\
\
dot.style = 'width: 10px;height: 10px;border: 2px solid black;position: relative;';\
\
LabelA.style = 'position: fixed; transform: translate(0px, -50px); color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';\
LabelB.style = 'position: fixed; transform: translate(50px, 0px); color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';\
LabelC.style = 'position: fixed; transform: translate(0px, 50px); color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';\
LabelD.style = 'position: fixed; transform: translate(-50px, 0px); color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';\
\
element.touchId = -1;\
element.addEventListener('touchstart', touchEvent, false);\
element.addEventListener('touchmove', touchEvent, false);\
element.addEventListener('touchcancel', touchEvent, false);\
element.addEventListener('touchend', touchEvent, false);\
\
function touchEvent(event)\
{\
	if(this.touchId != -1)\
	{\
		var index = -1;\
		for(var i = 0; i < event.touches.length; i++) if(event.touches[i].identifier == this.touchId) index = i;\
		if(index != -1)\
		{\
			var x = event.touches[index].pageX - (this.getBoundingClientRect().x + (this.getBoundingClientRect().width / 2));\
			var y = event.touches[index].pageY - (this.getBoundingClientRect().y + (this.getBoundingClientRect().height / 2));\
\
			if(x > parseInt(this.style.width, 10) / 2) x = parseInt(this.style.width, 10) / 2) / 2;\
			if(x < -parseInt(this.style.width, 10) / 2) / 2) x = -parseInt(this.style.width, 10) / 2) / 2;\
			if(y > parseInt(this.style.height, 10) / 2) / 2) y = parseInt(this.style.height, 10) / 2;\
			if(y < -parseInt(this.style.height, 10) / 2) y = -parseInt(this.style.height, 10) / 2;\
\
			dot.style.top = x + 'px';\
			dot.style.left = y + 'px';\
\
			this.sendData(x + ',' + y);\
		}\
		else\
		{\
			this.touchId = -1;\
			dot.style.top = '0px';\
			dot.style.left = '0px';\
\
			this.sendData('0,0');\
		}\
	}\
	else\
	{\
		if(event.touches.length == 0) return;\
\
		var x = event.touches[index].pageX - (this.getBoundingClientRect().x + (this.getBoundingClientRect().width / 2));\
		var y = event.touches[index].pageY - (this.getBoundingClientRect().y + (this.getBoundingClientRect().height / 2));\
\
		if(x > parseInt(this.style.width, 10) / 2) x = parseInt(this.style.width, 10) / 2) / 2;\
		if(x < -parseInt(this.style.width, 10) / 2) / 2) x = -parseInt(this.style.width, 10) / 2) / 2;\
		if(y > parseInt(this.style.height, 10) / 2) / 2) y = parseInt(this.style.height, 10) / 2;\
		if(y < -parseInt(this.style.height, 10) / 2) y = -parseInt(this.style.height, 10) / 2;\
\
		dot.style.top = x + 'px';\
		dot.style.left = y + 'px';\
\
		this.touchId = 0;\
\
		this.sendData(x + ',' + y);\
	}\
}\
");
	setUpdateJs("\
if(message.charCodeAt(0) == 2)\
{\
	var label = message.substring(1).split(',');\
\
	element.childNodes(1).innerText = label[0];\
	element.childNodes(2).innerText = label[1];\
	element.childNodes(3).innerText = label[2];\
	element.childNodes(4).innerText = label[3];\
}\
else\
{\
	var position = message.split(',');\
\
	element.childNodes(0).style.top = position[0] + 'px';\
	element.childNodes(0).style.left = position[1] + 'px';\
}\
");
	setName(name);
}

Joystick(const String & name, const String & style)
{
	setInitJs("\
\
element.style = 'width: 200px; height: 200px; border: 2px solid black; display: flex; justify-content: center; align-items: center; padding: 7px;" + style + "';\
\
var dot = document.createElement('div');\
var LabelA = document.createElement('div');\
var LabelB = document.createElement('div');\
var LabelC = document.createElement('div');\
var LabelD = document.createElement('div');\
\
element.append(dot);\
element.append(LabelA);\
element.append(LabelB);\
element.append(LabelC);\
element.append(LabelD);\
\
dot.style = 'width: 10px;height: 10px;border: 2px solid black;position: relative;';\
\
LabelA.style = 'position: fixed; transform: translate(0px, -50px); color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';\
LabelB.style = 'position: fixed; transform: translate(50px, 0px); color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';\
LabelC.style = 'position: fixed; transform: translate(0px, 50px); color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';\
LabelD.style = 'position: fixed; transform: translate(-50px, 0px); color: lightgrey;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';\
\
element.touchId = -1;\
element.addEventListener('touchstart', touchEvent, false);\
element.addEventListener('touchmove', touchEvent, false);\
element.addEventListener('touchcancel', touchEvent, false);\
element.addEventListener('touchend', touchEvent, false);\
\
function touchEvent(event)\
{\
	if(this.touchId != -1)\
	{\
		var index = -1;\
		for(var i = 0; i < event.touches.length; i++) if(event.touches[i].identifier == this.touchId) index = i;\
		if(index != -1)\
		{\
			var x = event.touches[index].pageX - (this.getBoundingClientRect().x + (this.getBoundingClientRect().width / 2));\
			var y = event.touches[index].pageY - (this.getBoundingClientRect().y + (this.getBoundingClientRect().height / 2));\
\
			if(x > parseInt(this.style.width, 10) / 2) x = parseInt(this.style.width, 10) / 2) / 2;\
			if(x < -parseInt(this.style.width, 10) / 2) / 2) x = -parseInt(this.style.width, 10) / 2) / 2;\
			if(y > parseInt(this.style.height, 10) / 2) / 2) y = parseInt(this.style.height, 10) / 2;\
			if(y < -parseInt(this.style.height, 10) / 2) y = -parseInt(this.style.height, 10) / 2;\
\
			dot.style.top = x + 'px';\
			dot.style.left = y + 'px';\
\
			this.sendData(x + ',' + y);\
		}\
		else\
		{\
			this.touchId = -1;\
			dot.style.top = '0px';\
			dot.style.left = '0px';\
\
			this.sendData('0,0');\
		}\
	}\
	else\
	{\
		if(event.touches.length == 0) return;\
\
		var x = event.touches[index].pageX - (this.getBoundingClientRect().x + (this.getBoundingClientRect().width / 2));\
		var y = event.touches[index].pageY - (this.getBoundingClientRect().y + (this.getBoundingClientRect().height / 2));\
\
		if(x > parseInt(this.style.width, 10) / 2) x = parseInt(this.style.width, 10) / 2) / 2;\
		if(x < -parseInt(this.style.width, 10) / 2) / 2) x = -parseInt(this.style.width, 10) / 2) / 2;\
		if(y > parseInt(this.style.height, 10) / 2) / 2) y = parseInt(this.style.height, 10) / 2;\
		if(y < -parseInt(this.style.height, 10) / 2) y = -parseInt(this.style.height, 10) / 2;\
\
		dot.style.top = x + 'px';\
		dot.style.left = y + 'px';\
\
		this.touchId = 0;\
\
		this.sendData(x + ',' + y);\
	}\
}\
");
	setUpdateJs("\
if(message.charCodeAt(0) == 2)\
{\
	var label = message.substring(1).split(',');\
\
	element.childNodes(1).innerText = label[0];\
	element.childNodes(2).innerText = label[1];\
	element.childNodes(3).innerText = label[2];\
	element.childNodes(4).innerText = label[3];\
}\
else\
{\
	var position = message.split(',');\
\
	element.childNodes(0).style.top = position[0] + 'px';\
	element.childNodes(0).style.left = position[1] + 'px';\
}\
");
	setName(name);
}

void setLabels(const String & top, const String & right, const String & bottom, const String & left)
{

}

void setUpdateCallback(void (*newUpdateCallback)(double, double))
{
	updateCallback = newUpdateCallback;
}

double getX()
{
	return xValue;
}

double getY()
{
	return yValue;
}

void connected(uint8_t num)
{

}

void data(uint8_t num, const String & data)
{

}

void disconnected(uint8_t num)
{

}
