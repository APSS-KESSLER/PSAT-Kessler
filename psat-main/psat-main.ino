#include <WiFi101.h>
#include <WiFiClient.h>
#include <WiFiMDNSResponder.h>
#include <WiFiServer.h>
#include <WiFiSSLClient.h>
#include <WiFiUdp.h>

WiFiServer server(80);

void setup() {
	WiFi.setPins(8, 7, 4, 2);

	Serial.begin(115200);
	while (!Serial);

	if (WiFi.status() == WL_NO_SHIELD) {
		Serial.println("No WiFi module found");

		while (true);
	}

	int status = WL_IDLE_STATUS;
	while (status != WL_CONNECTED) {
		Serial.println("Connecting...");
		Serial.println("SSID: APSSPSAT");

		status = WiFi.begin("APSSPSAT", "nopenope");

		delay(10000);
	}

	Serial.println("Connected!");

	Serial.print("Starting server on: ");
	Serial.println(IPAddress(WiFi.localIP()));

	server.begin();
}

void loop() {
	// listen for incoming clients
	WiFiClient client = server.available();
	if (client) {
		Serial.println("new client");
		// an http request ends with a blank line
		bool currentLineIsBlank = true;
		while (client.connected()) {
			if (client.available()) {
				char c = client.read();
				Serial.write(c);
				// if you've gotten to the end of the line (received a newline
				// character) and the line is blank, the http request has ended,
				// so you can send a reply
				if (c == '\n' && currentLineIsBlank) {
					// send a standard http response header
					client.println("HTTP/1.1 200 OK");
					client.println("Content-Type: text/html");
					client.println("Connection: close");	// the connection will be closed after completion of the response
					client.println("Refresh: 5");	// refresh the page automatically every 5 sec
					client.println();
					client.println("<!DOCTYPE HTML>");
					client.println("<html><b>Signal Strength: ");
					client.print(WiFi.RSSI());
					client.println("</b></html>");
					break;
				}
				if (c == '\n') {
					// you're starting a new line
					currentLineIsBlank = true;
				}
				else if (c != '\r') {
					// you've gotten a character on the current line
					currentLineIsBlank = false;
				}
			}
		}
		// give the web browser time to receive the data
		delay(1);

		// close the connection:
		client.stop();
		Serial.println("client disconnected");
	}
}

void printWiFiData() {
	// print your WiFi shield's IP address:
	IPAddress ip = WiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);

	// print your MAC address:
	byte mac[6];
	WiFi.macAddress(mac);
	Serial.print("MAC address: ");
	printMacAddress(mac);

}

void printCurrentNet() {
	// print the SSID of the network you're attached to:
	Serial.print("SSID: ");
	Serial.println(WiFi.SSID());

	// print the MAC address of the router you're attached to:
	byte bssid[6];
	WiFi.BSSID(bssid);
	Serial.print("BSSID: ");
	printMacAddress(bssid);

	// print the received signal strength:
	long rssi = WiFi.RSSI();
	Serial.print("signal strength (RSSI):");
	Serial.println(rssi);

	// print the encryption type:
	byte encryption = WiFi.encryptionType();
	Serial.print("Encryption Type:");
	Serial.println(encryption, HEX);
	Serial.println();
}

void printMacAddress(byte mac[]) {
	for (int i = 5; i >= 0; i--) {
		if (mac[i] < 16) {
			Serial.print("0");
		}
		
		Serial.print(mac[i], HEX);
		if (i > 0) {
			Serial.print(":");
		}
	}

	Serial.println();
}