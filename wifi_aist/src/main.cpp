#include "ESP8266WiFi.h" //which one of these should be used ?
#include "m2m_gateway_drv.h"
#include <Ticker.h>


const char* ssid = "Home Wi-Fi 2";
const char* password = "IUserThisWiFi";
//Static IP address configuration
/*
IPAddress staticIP(192, 168, 100, 50); //ESP static ip
IPAddress gateway(192, 168, 100, 1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(192, 168, 100, 1);  //DNS
*/

const char *deviceName = "wifi_m2m_gateway";
const uint16_t port = 10000;
const char* host = "192.168.100.5";

bool isInitM2m;
bool Timer1minFlag;

T_m2m_gateway m2m_gateway(1);
WiFiClient client;
Ticker Timer1min;

void Timer1minISR();

void setup()
{
	Timer1minFlag = false;
	Serial.begin(9600);
	delay(1000);
	WiFi.hostname(deviceName);     
	//WiFi.config(staticIP, dns , gateway, subnet);
	WiFi.disconnect();
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
}

void loop()
{
	if (!client.connected()) {
		Serial.print("Connecting to server ");
		Serial.println(host);
		while (!client.connect(host, port)) {
			Serial.println("Connection to host failed");
			delay(10000);
		}
		Serial.println("Connected to server successful!");
		isInitM2m = true;
	}
	
	if (isInitM2m) {
		Serial.println("Sending INIT packet to server");
		m2m_gateway.MakePacket(enumPacketType::INIT);
		client.write(m2m_gateway.TxRxBuffer.Buf, m2m_gateway.TxRxBuffer.BufSize);
		isInitM2m = false;
		Timer1min.attach(60, Timer1minISR);
	}

	if (Timer1minFlag) {
		Serial.println("Sending HeartBeat packet to server");
		m2m_gateway.MakePacket(enumPacketType::HEARTBEAT);
		client.write(m2m_gateway.TxRxBuffer.Buf, m2m_gateway.TxRxBuffer.BufSize);
		Timer1minFlag = false;
	};

	delay(500);
}

void Timer1minISR()
{
	Timer1minFlag = true;
	
}
