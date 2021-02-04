#include "psat-wifi.h"
#include "psat-data.h"
#include "psat-log.h"
#include "psat-rtc.h"
#include "psat-sd.h"
#include "psat-imu.h"
#include "psat-gps.h"

psat::WiFiModule wiFiModule;
psat::RtcModule rtcModule;
psat::CardModule cardModule;
psat::ImuModule imuModule;
psat::GpsModule gpsModule;

void setup() {
	Serial.begin(115200);
	while (!Serial);

	wiFiModule.setup();
	rtcModule.setup();
	cardModule.setup();
	imuModule.setup();
	gpsModule.setup();
}

bool putCallback(char const *url, long data) {
	LOG_INFO_P("Main"); Serial.print("Put request to '"); Serial.print(url); Serial.printf("' arg: %ld\n", data);

	if (strcmp(url, "/led") == 0) {
		return true;
	} else if (strcmp(url, "/sound") == 0) {
		return true;
	} else if (strcmp(url, "/camera") == 0) {
		return true;
	} else if (strcmp(url, "/time") == 0) {
		rtcModule.setTime(DateTime(static_cast<uint32_t>(data)));
		return true;
	} else {
		return false;
	}
}

void loop() {
	psat::Data data = { 0 };

	data.millis = millis();
	wiFiModule.writeData(data);
	rtcModule.writeData(data);
	imuModule.writeData(data);
	gpsModule.writeData(data);

	cardModule.writeToCard(data);
	wiFiModule.processClient(data, putCallback);
}
