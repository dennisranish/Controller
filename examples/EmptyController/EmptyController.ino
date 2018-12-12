#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Controller.h>

const char *ssid = "........";
const char *password = "........";

ESP8266WebServer server(80);
Controller controller({ "Drive Controller", "Arm Controller", "", "" }, 81);

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

	server.on("/", [](){server.send_P(200, "text/html", controller.getWebPage());});
	server.onNotFound([](){server.send(404, "text/plain", "Nothing here");});
	server.begin();
}

void loop(void)
{
	controller.update();
	server.handleClient();
}