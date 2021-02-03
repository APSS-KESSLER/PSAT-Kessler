#include "psat-wifi.h"

#include "psat-log.h"
#include "psat-http.h"

namespace psat {

WiFiModule::WiFiModule(uint16_t serverPort): 
	server(serverPort)
{ }

bool WiFiModule::setup(cstring wiFiName, cstring wiFiPassword, unsigned long timeoutMs) {
	configurePinsForBoard();
	configureLowPowerMode();
	
	if (!connectToWiFi(wiFiName, wiFiPassword, timeoutMs)) {
		return false;
	}

	startServer();

	return true;
}

void WiFiModule::configureLowPowerMode() {
	WiFi.lowPowerMode();
}

void WiFiModule::configurePinsForBoard() {
	WiFi.setPins(8, 7, 4, 2);
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
		LOG_ERROR_F("WiFi", "Failed to connect in %ulms (%d)", timeoutMs, WiFi.status());
		return false;
	}

	LOG_INFO_P("WiFi"); Serial.print("Connected with IP '"); Serial.print(IPAddress(WiFi.localIP())); Serial.println("'\n");

	server.begin();

	return true;
}

void WiFiModule::startServer() {
	server.begin();
}

void WiFiModule::processClient(psat::Data const &data) {
	if (WiFiClient client = server.available()) {
		psat::HTTPSession session(client);
		session.process(data);
	}
}

}