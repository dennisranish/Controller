#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Controller.h>
#include <LiveElement/Joystick.h>
#include <LiveElement/Button.h>
#include <LiveElement/Text.h>
#include <Servo.h>

bool useAP = false;
bool driveType = false; // defaults to tank
const int analog_ip = A0;

Servo myservoD2;
Servo myservoD5;
Servo myservoD6;
Servo myservoD8;

const char * ssid = "";
const char * password = "";

ESP8266WebServer server(80);
Controller controller({ "Drive Controller", "Arm Controller", "", "" }, 81);

Button * tankButton = new Button("tankButton", "Tank Mode", "position:absolute;left: calc(50vw - 150px);top:50px;");
Button * arcadeButton = new Button("arcadeButton", "Arcade Mode", "position:absolute;left: calc(50vw + 50px);top:50px;");

Button * trackedButton = new Button("trackedButton", "Tracks", "position:absolute;left: calc(50vw - 150px);bottom:calc(50vh - 25px);");
Button * omniButton = new Button("omniButton", "Omnis", "position:absolute;left: calc(50vw + 50px);bottom:calc(50vh - 25px);");

Text * textDriveMode = new Text("textDriveMode", "position:absolute;left: calc(50vw - 40px);top:25px;");

void setup(void)
{
	Serial.begin(115200);
	Serial.println("");

	if(useAP)
	{
		WiFi.mode(WIFI_AP);
		WiFi.softAP(ssid, password);

		Serial.print("Connect to: ");
		Serial.println(ssid);
		Serial.print("Password: ");
		Serial.println(password);
		Serial.print("Soft-AP IP address: ");
		Serial.println(WiFi.softAPIP());
	}
	else
	{
		WiFi.mode(WIFI_STA);
		WiFi.begin(ssid, password);

		while (WiFi.status() != WL_CONNECTED)
		{
			delay(500);
			Serial.print(".");
		}
		Serial.println("");
		Serial.print("Connect to: ");
		Serial.println(ssid);
		Serial.print("IP address: ");
		Serial.println(WiFi.localIP());
	}

	server.on("/", [](){server.send_P(200, "text/html", controller.getWebPage());});
	server.onNotFound([](){server.send(404, "text/plain", "Nothing here");});
	server.begin();

	myservoD2.attach(04); // D2 = GPIO04
	myservoD5.attach(12); // D5 = GPIO14
	myservoD6.attach(14); // D6 = GPIO12
	myservoD8.attach(15); // D8 = GPIO15

	controller[0].add(new Joystick("leftJoystick", "width: 150px;height: 150px;position: absolute;left: 0;bottom: 0;"));
	controller[0].add(new Joystick("rightJoystick", "width: 150px;height: 150px;position: absolute;right: 0;bottom: 0;"));
	controller[0].add(tankButton);
	controller[0].add(arcadeButton);
	controller[0].add(trackedButton);
	controller[0].add(omniButton);
	controller[0].add(textDriveMode);

	textDriveMode->setText("Mode: Tank Drive");
}

int lastConnected = -1;
int lastInputVal = -1;
int lastShiftPwr = 0;

int pwr2srv(int pwr)
{
	if(pwr > 100) pwr = 100;
	if(pwr < -100) pwr = -100;
	return (pwr + 100.0) * 0.9; // 0-180 deg
}

void loop(void)
{
	if(useAP)
	{
		int numConnected = WiFi.softAPgetStationNum();

		if(numConnected != lastConnected)
		{
			Serial.printf("Stations connected = %d\n", numConnected);
		}

		lastConnected = numConnected;
	}

	controller.update();
	server.handleClient();

	if(tankButton->getValue() && driveType != false)
	{
		driveType = false;
		textDriveMode->setText("Mode: Tank Drive"); 
	}

	if(arcadeButton->getValue() && driveType != true)
	{
		driveType = true;
		textDriveMode->setText("Mode: Split Arcade Drive");
	}

	int inputVal = analogRead(analog_ip); // Analog Values 0 to 1023

	if(abs(inputVal-lastInputVal) > 10)
	{
		Serial.println (inputVal);
		//lastInputVal = inputVal;
	}

	int j0Y = controller[0].e<Joystick>(0)->y();
	int j0X = controller[0].e<Joystick>(0)->x();
	int j1Y = controller[0].e<Joystick>(1)->y();
	int j1X = controller[0].e<Joystick>(1)->x();

	int pwrL = j0Y;
	int pwrR = j1Y;

	if(driveType) //arcade
	{
		pwrL = j1Y - j0X/2;
		pwrR = j1Y + j0X/2;
	}

	int L1 = pwrL;
	int L2 = pwrL;
	int R1 = pwrR;
	int R2 = pwrR;

	if(trackedButton->getValue())
	{
		int shiftPwr = inputVal - 0;

		if(shiftPwr > 200) shiftPwr = 200;

		if(shiftPwr > lastShiftPwr)
		{
			shiftPwr = ++lastShiftPwr;
		}

		shiftPwr /= 2;
		L1 = shiftPwr;
		L2 = -L1;
		R1 = shiftPwr;
		R2 = -R1;
	}
	else if(omniButton->getValue())
	{
		int shiftPwr = 1023 - inputVal;

		if(shiftPwr > 200) shiftPwr = 200;

		if(shiftPwr > lastShiftPwr)
		{
			shiftPwr = ++lastShiftPwr;
		}

		shiftPwr /= 2;
		
		if(inputVal != lastInputVal || shiftPwr != lastShiftPwr)
		{
			//Serial.printf("input=%d  power=%d\n",inputVal,shiftPwr);
		}

		L1 = -shiftPwr;
		L2 = -L1;
		R1 = -shiftPwr;
		R2 = -R1;
	}
	else
	{
		lastShiftPwr=0;
		//leave defaults;
	}

	myservoD2.write(pwr2srv(L1));
	myservoD5.write(pwr2srv(L2));
	myservoD6.write(pwr2srv(-R1));
	myservoD8.write(pwr2srv(-R2));

	lastInputVal = inputVal;

	delay(1); 
}
