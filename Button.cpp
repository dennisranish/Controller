#include <Button.h>

Button::Button(String name, String title)
{
	setInitJs("");
	setUpdateJs("element.innerText = message");
	setName(name);
}

Button::Button(String name, String title, String style, String id, String classList)
{
	setInitJs("element.innerText = '" + title + "'; element.id = '" + id + "'; element.style = '" + style + "'; element.classList = '" + classList + "';");
	setUpdateJs("element.innerText = message");
	setName(name);
}

void Button::setTitle(String newTitle)
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

void Button::data(uint8_t num, String data)
{
	if(data == "pressed")
	{
		value = true;
		if(changeCallback != NULL) changeCallback(true);
		if(pressedCallback != NULL) pressedCallback();
	}
	else if(data == "unpressed")
	{
		value = false;
		if(changeCallback != NULL) changeCallback(false);
		if(unpressedCallback != NULL) unpressedCallback();
	}
}

void Button::disconnected(uint8_t num)
{

}