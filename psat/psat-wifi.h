#ifndef PSAT_WIFI_H
#define PSAT_WIFI_H

#include <WiFi101.h>

#include "psat-types.h"

namespace psat {

struct Data;

class WiFiModule {
private:
	WiFiServer server;
	
public:
	WiFiModule(uint16_t serverPort = 80);
	
	bool setup(cstring wiFiName = "APSSPSAT", cstring wiFiPassword = "nopenope", unsigned long timeoutMs = 10000);
	void processClient(psat::Data const &data, PutCallback putCallback);
	void writeData(psat::Data &data);
private:
	void configureLowPowerMode();
	void configurePinsForBoard();
	bool connectToWiFi(cstring wiFiName, cstring wiFiPassword, unsigned long timeoutMs);
	void startServer();
};

}

#endif