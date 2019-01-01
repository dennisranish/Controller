#ifndef Element_h
#define Element_h

class Element;
class Controller;
class SingleController;

#include <Arduino.h>

class Element
{
	public:
		void display(bool visible);
		void display(char* type);

	protected:
		void setInitJs(char* newInitJs);
		void setUpdateJs(char* newInitJs);
		void setName(char* newInitJs);
		void setInitJs(std::vector<char*> newInitJs);
		void setUpdateJs(std::vector<char*> newInitJs);
		virtual void connected(uint8_t num) = 0;
		virtual void data(uint8_t num, char* data) = 0;
		virtual void disconnected(uint8_t num) = 0;
		void sendData(uint8_t num, char* data);
		void broadcastData(char* data);

	private:
		SingleController* parentController;
		std::vector<char*> initJs;
		std::vector<char*> updateJs;
		char* name;

		friend class Controller;
		friend class SingleController;
};

#endif