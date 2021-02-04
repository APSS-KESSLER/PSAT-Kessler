#include "psat-wifi.h"
#include "psat-data.h"
#include "psat-log.h"
#include "psat-rtc.h"

psat::WiFiModule wiFiModule;
psat::RtcModule rtcModule;

void setup() {
	Serial.begin(115200);
	while (!Serial);

	wiFiModule.setup();
	rtcModule.setup();
}

bool putCallback(char const *url, long data) {
	LOG_INFO_P("Main"); Serial.print("Put request to '"); Serial.print(url); Serial.printf("' arg: %ld\n", data);

	if (strcmp(url, "/led") == 0) {

	} else if (strcmp(url, "/sound") == 0) {

	} else if (strcmp(url, "/camera") == 0) {

	} else if (strcmp(url, "/time") == 0) {
		rtcModule.setTime(DateTime(static_cast<uint32_t>(data)));
	} else {
		return false;
	}
}

void loop() {
	psat::Data data;

	wiFiModule.writeData(data);
	rtcModule.writeData(data);

	wiFiModule.processClient(data, putCallback);
}
