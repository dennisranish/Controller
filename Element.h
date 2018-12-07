#ifndef Element_h
#define Element_h

class Element;
class LiveWebPage;

#include <Arduino.h>

class Element
{
	public:

	protected:
		void setInitJs(const String & newInitJs);
		void setUpdateJs(const String & newInitJs);
		void setName(const String & newInitJs);
		virtual void connected(uint8_t num) = 0;
		virtual void data(uint8_t num, const String & data) = 0;
		virtual void disconnected(uint8_t num) = 0;
		void sendData(uint8_t num, const String & data);
		void broadcastData(const String & data);

	private:
		LiveWebPage* parentPage;
		String initJs = "";
		String updateJs = "";
		String name = "";

		friend class LiveWebPage;
};

#endif