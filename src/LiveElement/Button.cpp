#include "Button.h"

char* Button::jsInitCode = R"(
element.style.cssText += 'border: 1px solid black;background: white;margin: 10px;padding: 10px;width: fit-content;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';
element.state = 'unpressed';
element.setPressed = function(isSendData) { this.state = 'pressed'; this.style.border = '1px solid #2ba9ff'; this.style.background = '#2ba9ff'; if(isSendData == true) element.sendData('pressed'); };
element.setUnpressed = function(isSendData) { this.state = 'unpressed'; this.style.border = '1px solid black'; this.style.background = 'white'; if(isSendData == true) element.sendData('unpressed'); };
document.addEventListener('mousedown', mouseevent);
document.addEventListener('mouseup', mouseevent);
document.addEventListener('mousemove', mouseevent);

function mouseevent(e)
{
	if(e.path[0] == element && e.buttons > 0 && e.button == 0) if(this.state == 'unpressed') this.setPressed(true);
	else if(this.state == 'pressed') this.setUnpressed(true);
}

document.addEventListener('touchstart', touchevent);
document.addEventListener('touchmove', touchevent);
document.addEventListener('touchend', touchevent);

function touchevent(e)
{
	for(var i = 0; i < e.touches.length; i++)
	{
		if(e.touches[i].pageX >= element.getBoundingClientRect().left &&
		e.touches[i].pageX <= element.getBoundingClientRect().right &&
		e.touches[i].pageY >= element.getBoundingClientRect().top &&
		e.touches[i].pageY <= element.getBoundingClientRect().bottom)
		{
			if(foundTouch) if(this.state == 'unpressed') this.setPressed(true);
			else if(this.state == 'pressed') this.setUnpressed(true);
			break;
		}
	}
}
)";

char* Button::jsUpdateCode = R"(
if(message.charCodeAt(0) == 2)
{
	element.innerText = message.substring(1);
}
else if(!isOwner)
{
	if(message == 'pressed') element.setPressed(false);
	if(message == 'unpressed') element.setUnpressed(false);
}
)";

Button::Button(char* name, char* title)
{
	setInitJs({ (char*)"element.innerText = '", title, (char*)"';", jsInitCode });
	setUpdateJs(jsUpdateCode);
	setName(name);
}

Button::Button(char* name, char* title, char* style)
{
	setInitJs({ (char*)"element.innerText = '", title, (char*)"';", (char*)"element.style = '", style, (char*)"';", jsInitCode });
	setUpdateJs(jsUpdateCode);
	setName(name);
}

void Button::setTitle(char* newTitle)
{
	title = newTitle;

	char* stringA = FastString::add({ (char*)"\x002", title });
	broadcastData(stringA);
	free(stringA);
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

void Button::connected(uint8_t num)
{

}

void Button::data(uint8_t num, char* data)
{
	if(strcmp(data, "pressed") == 0 && value != true)
	{
		value = true;
		if(changeCallback != NULL) changeCallback(true);
		if(pressedCallback != NULL) pressedCallback();
		broadcastData("\x002pressed");
	}
	else if(strcmp(data, "unpressed") == 0 && value != false)
	{
		value = false;
		if(changeCallback != NULL) changeCallback(false);
		if(unpressedCallback != NULL) unpressedCallback();
		broadcastData("\x002unpressed");
	}
}

void Button::disconnected(uint8_t num)
{

}