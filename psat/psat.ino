#include "psat-wifi.h"
#include "psat-data.h"

psat::WiFiModule wiFiModule;

void setup() {
	Serial.begin(115200);
	while (!Serial);

	wiFiModule.setup();
}

void loop() {
	psat::Data data;

	wiFiModule.processClient(data);
}
