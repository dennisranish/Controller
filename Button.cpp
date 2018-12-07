#include <Button.h>

Button::Button(const String & name, const String & title)
{
	setInitJs("element.state = 'unpressed';\
document.addEventListener('mousedown', mouseevent);\
document.addEventListener('mouseup', mouseevent);\
document.addEventListener('mousemove', mouseevent);\
\
function mouseevent(e)\
{\
	if(e.path[0] == element && e.buttons > 0 && e.button == 0)\
	{\
		if(element.state == 'unpressed')\
		{\
			element.state = 'pressed';\
			element.sendData('pressed');\
		}\
	}\
	else\
	{\
		if(element.state == 'pressed')\
		{\
			element.state = 'unpressed';\
			element.sendData('unpressed');\
		}\
	}\
}\
\
document.addEventListener('touchstart', touchevent);\
document.addEventListener('touchmove', touchevent);\
document.addEventListener('touchend', touchevent);\
\
function touchevent(e)\
{\
	var foundTouch = false;\
\
	for(var i = 0; i < e.touches.length; i++)\
	{\
		if(e.touches[i].pageX >= element.getBoundingClientRect().left &&\
		e.touches[i].pageX <= element.getBoundingClientRect().right &&\
		e.touches[i].pageY >= element.getBoundingClientRect().top &&\
		e.touches[i].pageY <= element.getBoundingClientRect().bottom)\
		{\
			foundTouch = true;\
			break;\
		}\
	}\
\
	if(foundTouch)\
	{\
		if(element.state == 'unpressed')\
		{\
			element.state = 'pressed';\
			element.sendData('pressed');\
		}\
	}\
	else\
	{\
		if(element.state == 'pressed')\
		{\
			element.state = 'unpressed';\
			element.sendData('unpressed');\
		}\
	}\
}\
element.style = 'width: fit-content;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';");
	setUpdateJs("element.innerText = message");
	setName(name);
}

Button::Button(const String & name, const String & title, const String & style, const String & id, const String & classList)
{
	setInitJs("element.state = 'unpressed';\
document.addEventListener('mousedown', mouseevent);\
document.addEventListener('mouseup', mouseevent);\
document.addEventListener('mousemove', mouseevent);\
\
function mouseevent(e)\
{\
	if(e.path[0] == element && e.buttons > 0 && e.button == 0)\
	{\
		if(element.state == 'unpressed')\
		{\
			element.state = 'pressed';\
			element.sendData('pressed');\
		}\
	}\
	else\
	{\
		if(element.state == 'pressed')\
		{\
			element.state = 'unpressed';\
			element.sendData('unpressed');\
		}\
	}\
}\
\
document.addEventListener('touchstart', touchevent);\
document.addEventListener('touchmove', touchevent);\
document.addEventListener('touchend', touchevent);\
\
function touchevent(e)\
{\
	var foundTouch = false;\
\
	for(var i = 0; i < e.touches.length; i++)\
	{\
		if(e.touches[i].pageX >= element.getBoundingClientRect().left &&\
		e.touches[i].pageX <= element.getBoundingClientRect().right &&\
		e.touches[i].pageY >= element.getBoundingClientRect().top &&\
		e.touches[i].pageY <= element.getBoundingClientRect().bottom)\
		{\
			foundTouch = true;\
			break;\
		}\
	}\
\
	if(foundTouch)\
	{\
		if(element.state == 'unpressed')\
		{\
			element.state = 'pressed';\
			element.sendData('pressed');\
		}\
	}\
	else\
	{\
		if(element.state == 'pressed')\
		{\
			element.state = 'unpressed';\
			element.sendData('unpressed');\
		}\
	}\
}\
				element.innerText = '" + title + "';\
				element.id = '" + id + "';\
				element.style = '" + style + "width: fit-content;-webkit-touch-callout: none;-webkit-user-select: none;-khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;';\
				element.classList = '" + classList + "';");
	setUpdateJs("element.innerText = message");
	setName(name);
}

void Button::setTitle(const String & newTitle)
{
	title = newTitle;
	broadcastData(title);
}

String Button::getTitle()
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

void Button::data(uint8_t num, const String & data)
{
	if(data == "pressed" && value != true)
	{
		value = true;
		if(changeCallback != NULL) changeCallback(true);
		if(pressedCallback != NULL) pressedCallback();
	}
	else if(data == "unpressed" && value != false)
	{
		value = false;
		if(changeCallback != NULL) changeCallback(false);
		if(unpressedCallback != NULL) unpressedCallback();
	}
}

void Button::disconnected(uint8_t num)
{

}