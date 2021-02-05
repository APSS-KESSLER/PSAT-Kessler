#include "psat-wifi.h"

#include "psat-log.h"
#include "psat-http.h"
#include "psat-data.h"

namespace psat {

WiFiModule::WiFiModule(uint16_t serverPort): 
	server(serverPort)
{ }

void WiFiModule::writeData(psat::Data &data) {
	if (!valid) {
		return;
	}

	data.rssi = WiFi.RSSI();
}

void WiFiModule::setup(cstring wiFiName, cstring wiFiPassword, unsigned long timeoutMs) {
	configurePinsForBoard();
	configureLowPowerMode();
	
	if (!connectToWiFi(wiFiName, wiFiPassword, timeoutMs)) {
		return;
	}

	startServer();

	valid = true;
}

void WiFiModule::configureLowPowerMode() {
	WiFi.lowPowerMode();
}

void WiFiModule::configurePinsForBoard() {
	WiFi.setPins(8, 7, 4, 2);
}

void WiFiModule::disconnect() {
	WiFi.disconnect();
}

bool WiFiModule::connectToWiFi(cstring wiFiName, cstring wiFiPassword, unsigned long timeoutMs) {
	if (WiFi.status() == WL_NO_SHIELD) {
		LOG_ERROR("WiFi", "No WiFi module found");

		return false;
	}

	LOG_INFO_F("WiFi", "Firmware version: %s", WiFi.firmwareVersion());
	LOG_INFO_F("WiFi", "Connecting to %s", wiFiName);

	WiFi.setTimeout(timeoutMs);
	
	if (WiFi.begin(wiFiName, wiFiPassword) != WL_CONNECTED) {
		LOG_ERROR_F("WiFi", "Failed to connect in %lums (%d)", timeoutMs, WiFi.status());
		return false;
	}

	LOG_INFO_P("WiFi"); Serial.print("Connected with IP '"); Serial.print(IPAddress(WiFi.localIP())); Serial.println("'");

	server.begin();

	return true;
}

void WiFiModule::startServer() {
	server.begin();
}

void WiFiModule::processClient(psat::Data const &data, PutCallback putCallback) {
	if (!valid) {
		if (!connectToWiFi()) {
			return;
		}

		valid = true;
	}

	if (WiFiClient client = server.available()) {
		psat::HTTPSession session(client);
		session.process(data, putCallback);
	}
}

}