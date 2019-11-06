#include "ESP8266WiFi.h" //which one of these should be used ?
#include "m2m_gateway_drv.h"
#include <Ticker.h>

const char* ssid = "Home Wi-Fi 2";
const char* password = "Barikina321bLab";
//Static IP address configuration
IPAddress staticIP(192, 168, 100, 100); //ESP static ip
IPAddress gateway(192, 168, 100, 1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(192, 168, 100, 1);  //DNS

const char *deviceName = "wifi_m2m_gateway";

const uint16_t port = 10000;
const char* host = "192.168.100.6";

char m2m_init[] = {	0x24, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
					0x23, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
					0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
					0x31, 0x31, 0xff, 0x01, 0x32, 0x32, 0x32,
					0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
					0x32, 0x32, 0x32, 0x32, 0x32, 0xff, 0x01,
					0x01, 0x88, 0x9d};

T_m2m_gateway m2m_gateway(1);
bool isInitM2m = true;
bool isNotConnect = true;
Ticker Timer1min;

void setup()
{
	Serial.begin(9600);
	delay(1000);
	WiFi.hostname(deviceName);     
	WiFi.config(staticIP, dns , gateway, subnet);
	WiFi.begin ( ssid, password );
	Serial.println ( "" );

	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay ( 500 );
		Serial.print ( "." );
	}

	Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( ssid );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );
	Serial.print("Subnet Mask ");
	Serial.println(WiFi.subnetMask());
	Serial.print("MAC: ");
	Serial.println(WiFi.macAddress());
	Serial.print("Gateway ");
	Serial.println(WiFi.gatewayIP());
	Serial.print("DNS: ");
	Serial.println(WiFi.dnsIP());
	Serial.print("Channel: ");
	Serial.println(WiFi.channel());
	Serial.print("Status : ");
	Serial.println(WiFi.status());
	
	//Serial.printf("Started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
	Timer1min.attach(60, Timer1minISR);
}

void loop()
{
	WiFiClient client;
	if (!client.connected()) {
		while (!client.connect(host, port)) {
			Serial.println("Connection to host failed");
			bool isInitM2m = true;
			delay(5000);
			Serial.println("Connected to server successful!");
			//return;
		}
	}
	
	if (isInitM2m) {
		m2m_gateway.MakePacket(enumPacketType::INIT);
		client.write(m2m_gateway.TxRxBuffer.Buf, m2m_gateway.TxRxBuffer.BufSize);
		isInitM2m = false;
	}

	delay(5000);
}



/*
#include "WebSocketsClient.h"
#include "ESP8266WiFi.h"
#include "m2m_gateway_drv.h"

WebSocketsClient webSocket;
int m2m_port(10000);
char m2m_addr[] = "192.168.1.84";
char m2m_init[] = {	0x24, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
					0x23, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
					0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
					0x31, 0x31, 0x7f, 0x01, 0x32, 0x32, 0x32,
					0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
					0x32, 0x32, 0x32, 0x32, 0x32, 0x7f, 0x00,
					0x01, 0xea, 0x0d};
String s(m2m_init);

#define USE_SERIAL Serial

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

	switch(type) {
		case WStype_DISCONNECTED:
			USE_SERIAL.printf("[WSc] Disconnected!\n");
			break;
		case WStype_CONNECTED: {
			USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);
			
			// send message to server when Connected
			webSocket.sendTXT("Connected");
			webSocket.sendTXT(m2m_init);
		}
			break;
		case WStype_TEXT:
			USE_SERIAL.printf("[WSc] get text: %s\n", payload);

			// send message to server
			// webSocket.sendTXT("message here");
			break;
		case WStype_BIN:
			USE_SERIAL.printf("[WSc] get binary length: %u\n", length);
			hexdump(payload, length);

			// send data to server
			// webSocket.sendBIN(payload, length);
			break;
        case WStype_PING:
            // pong will be send automatically
            USE_SERIAL.printf("[WSc] get ping\n");
            break;
        case WStype_PONG:
            // answer to a ping we send
            USE_SERIAL.printf("[WSc] get pong\n");
            break;
    }

}

void setup() {

	USE_SERIAL.begin(9600);

	//Serial.setDebugOutput(true);
	//USE_SERIAL.setDebugOutput(true);

	USE_SERIAL.println();
	USE_SERIAL.println();
	USE_SERIAL.println();

	for(uint8_t t = 4; t > 0; t--) {
		USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
		USE_SERIAL.flush();
		delay(1000);
	}

	WiFi.begin("SMOEFI", "Barikina321bLab");
	//WiFi.begin("Home Wi-Fi 2", "IUserThisWiFi");

	Serial.print("Connecting");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	webSocket.begin(m2m_addr, m2m_port, "/");

	// event handler
	webSocket.onEvent(webSocketEvent);

	// use HTTP Basic Authorization this is optional remove if not needed
	//webSocket.setAuthorization("user", "Password");

	// try ever 5000 again if connection has failed
	webSocket.setReconnectInterval(5000);
  
  // start heartbeat (optional)
  // ping server every 15000 ms
  // expect pong from server within 3000 ms
  // consider connection disconnected if pong is not received 2 times
  webSocket.enableHeartbeat(15000, 3000, 2);
}

void loop() {
webSocket.loop();
}
*/