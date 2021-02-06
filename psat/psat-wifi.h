#ifndef PSAT_WIFI_H
#define PSAT_WIFI_H

#include <WiFi101.h>

#include "psat-types.h"

namespace psat {

struct Data;

class WiFiModule {
	WiFiServer server;
	
public:
	WiFiModule(uint16_t serverPort = 80);
	
	void setup(cstring wiFiName = "APSSPSAT", cstring wiFiPassword = "nopenope", unsigned long timeoutMs = 10000);
	void processClient(psat::Data const &data, PutCallback putCallback);
	void writeData(psat::Data &data);
	void disconnect();
private:
	void configureLowPowerMode();
	void configurePinsForBoard();
	bool connectToWiFi(cstring wiFiName = "APSSPSAT", cstring wiFiPassword = "nopenope", unsigned long timeoutMs = 10000);
	void startServer();
};

}

#endif