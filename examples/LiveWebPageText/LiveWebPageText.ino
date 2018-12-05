#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LiveWebPage.h>
#include <LiveElement/Text.h>

const char *ssid = "........";
const char *password = "........";

ESP8266WebServer server(80);
LiveWebPage liveWebPage(81);

long lastTimeLog = 0;

void setup(void)
{
	Serial.begin(115200);
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	Serial.println("");
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	server.on("/", [](){server.send_P(200, "text/html", liveWebPage.getWebPage());});
	server.onNotFound([](){server.send(404, "text/plain", "Nothing here");});
	server.begin();

	liveWebPage.add(new Text("Time log", "margin: 10px; border: 1px solid black; padding: 10px; font: 25px sans-serif;"));
}

void loop(void)
{
	if(millis() - lastTimeLog > 1000)
	{
		liveWebPage.e<Text>("Time log").setText("esp time in milliseconds: " + String(millis()));
		lastTimeLog = millis();
	}

	liveWebPage.update();
	server.handleClient();
}