#ifndef Element_h
#define Element_h

class Element;

#include <Arduino.h>
#include "LiveWebPage.h"

class Element
{
	public:

	protected:
		void setInitJs(String newInitJs);
		void setUpdateJs(String newInitJs);
		void setName(String newInitJs);
		virtual void connected(uint8_t num) = 0;
		virtual void data(uint8_t num, String data) = 0;
		virtual void disconnected(uint8_t num) = 0;
		void sendData(uint8_t num, String data);
		void broadcastData(String data);

	private:
		std::vector<LiveWebPage*> parentPage;
		String initJs = "";
		String updateJs = "";
		String name = "";

		friend class LiveWebPage;
};

#endif