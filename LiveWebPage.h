#ifndef LiveWebPage_h
#define LiveWebPage_h

class LiveWebPage;

#include <Arduino.h>
#include <WebSocketsServer.h>
#include <Element.h>

class LiveWebPage
{
	public:
		LiveWebPage(int setPort);
		void add(Element *addElement);
		Element* e(int index);
		template <typename T> T* e(int index)
		{
			return (T*)element[index];
		}
		Element* e(const String & name);
		template <typename T> T* e(const String & name)
		{
			int index = -1;
			for(int i = 0; i < element.size(); i++) if(element[i]->name == name) { index = i; break; }
			if(index == -1) return NULL;
			return (T*)element[index];
		}
		const char* getWebPage();
		void update();
		void remove(Element *removeElement);

		//SingleController &operator[] (int index);

	private:
		int port;
		static String webPage0;
		static String webPage1;
		String webPage;
		WebSocketsServer webSocket;
		std::vector<Element*> element;

		void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
		void sendData(uint8_t num, Element *elementId, const String & data);
		void broadcastData(Element *elementId, const String & data);

		friend class Element;
};

//js adds html and handles events (uses function send message)
//js function to proccess input
//cpp function to proccess output
//otherStuff (can send out messages)
////all parentPages.send('message');
//stuff will be append to message on way out but remove on way in
//message will be exacly the same on other side to js

#endif

//classes
//LiveWebPage
//Element
//Controller (possibly extends container)
//Container

//LiveWebPage
//fullscreen containers
//transitions

//Elements are create a containers and their js adds stuff
//Elements anytime toggle visibility

//LiveWebPage liveWebPage(81);
//Controller controller(3);

//liveWebPage.add(controller);
//controller[0].add(new Joystick());
//controller[0].add(new Joystick());
//controller[1].add(new JoystickHorizantal());
//controller[1].add(new JoystickHorizantal());