#include "psat-wifi.h"
#include "psat-data.h"
#include "psat-log.h"

psat::WiFiModule wiFiModule;

void setup() {
	Serial.begin(115200);
	while (!Serial);

	wiFiModule.setup();
}

bool putCallback(char const *url, long data) {
	LOG_INFO_P("Main"); Serial.print("Put request to '"); Serial.print(url); Serial.printf("' arg: %ld\n", data);

	return true;
}

void loop() {
	psat::Data data;

	wiFiModule.writeData(data);

	wiFiModule.processClient(data, putCallback);
}
