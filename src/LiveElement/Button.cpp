#include "Button.h"

char* Button::jsCode = R"(
element.style.cssText += 'border: 1px solid black;background: white;margin: 10px;padding: 10px;width: fit-content;font-style: monospace;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';
element.state = 'unpressed';
element.setPressed = function(sendData) { this.state = 'pressed'; this.style.border = '1px solid #2ba9ff'; this.style.background = '#2ba9ff'; this.style.color = '#ffffff'; if(sendData) this.send('pressed'); };
element.setUnpressed = function(sendData) { this.state = 'unpressed'; this.style.border = '1px solid black'; this.style.background = 'white'; this.style.color = '#000000'; if(sendData) this.send('unpressed'); };
document.addEventListener('mousedown', mouseevent);
document.addEventListener('mouseup', mouseevent);
document.addEventListener('mousemove', mouseevent);
let savedElement = element;

function mouseevent(e)
{
	if(e.path[0] == savedElement && e.buttons > 0 && e.button == 0) { if(this.state == 'unpressed') this.setPressed(true); }
	else { if(this.state == 'pressed') this.setUnpressed(true); }
}

document.addEventListener('touchstart', touchevent);
document.addEventListener('touchmove', touchevent);
document.addEventListener('touchend', touchevent);

function touchevent(e)
{
	var foundTouch = false;

	for(var i = 0; i < e.touches.length; i++)
	{
		if(e.touches[i].pageX >= savedElement.getBoundingClientRect().left &&
		e.touches[i].pageX <= savedElement.getBoundingClientRect().right &&
		e.touches[i].pageY >= savedElement.getBoundingClientRect().top &&
		e.touches[i].pageY <= savedElement.getBoundingClientRect().bottom)
		{
			foundTouch = true;
			break;
		}
	}

	if(foundTouch) { if(savedElement.state == 'unpressed') savedElement.setPressed(true); }
	else { if(savedElement.state == 'pressed') savedElement.setUnpressed(true); }
}
)";

Button::Button(char* name, char* title, char* style)
{
	this->name = name;
	this->title = title;
	this->style = style;
}

void Button::setTitle(char* newTitle)
{
	title = newTitle;

	broadcastSelectSelf();
	broadcastData("element.innerText = '");
	broadcastData(title);
	broadcastData("';");
	broadcastRun();
}

const char* Button::getTitle()
{
	return title;
}

bool Button::getValue()
{
	return value;
}

void Button::setChangeCallback(void (*newChangeCallback)(bool))
{
	changeCallback = newChangeCallback;
}

void Button::setPressedCallback(void (*newPressedCallback)())
{
	pressedCallback = newPressedCallback;
}

void Button::setUnpressedCallback(void (*newUnpressedCallback)())
{
	unpressedCallback = newUnpressedCallback;
}

void Button::connectedEvent(uint8_t num)
{
	selectSelf(num);
	sendData(num, "element.innerText = '");
	sendData(num, title);
	sendData(num, "';");
	sendData(num, "element.style = '");
	sendData(num, style);
	sendData(num, "';");
	sendData(num, jsCode);
	sendRun(num);
}

void Button::dataEvent(uint8_t num, char* data)
{
	if(strcmp(data, "pressed") == 0 && value != true)
	{
		value = true;
		if(changeCallback != NULL) changeCallback(true);
		if(pressedCallback != NULL) pressedCallback();
		broadcastSelectSelf();
		broadcastData("element.setPressed(false);");
		broadcastRun();
	}
	else if(strcmp(data, "unpressed") == 0 && value != false)
	{
		value = false;
		if(changeCallback != NULL) changeCallback(false);
		if(unpressedCallback != NULL) unpressedCallback();
		broadcastSelectSelf();
		broadcastData("element.setUnpressed(false);");
		broadcastRun();
	}
}

void Button::disconnectedEvent(uint8_t num)
{

}