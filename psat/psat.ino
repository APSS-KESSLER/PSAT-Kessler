#include "psat-wifi.h"
#include "psat-data.h"
#include "psat-log.h"
#include "psat-rtc.h"
#include "psat-sd.h"
#include "psat-imu.h"
#include "psat-gps.h"
#include "psat-bmp.h"
#include "psat-camera.h"

psat::WiFiModule wiFiModule;
psat::RtcModule rtcModule;
psat::CardModule cardModule;
psat::ImuModule imuModule;
psat::GpsModule gpsModule;
psat::BmpModule bmpModule;
psat::CameraModule cameraModule;

void setup() {
	Serial.begin(115200);
	while (!Serial);

	cameraModule.setup();
	wiFiModule.setup();
	rtcModule.setup();
	cardModule.setup();
	imuModule.setup();
	gpsModule.setup();
	bmpModule.setup();

	bmpModule.setKnownAltitude(0.0f);
}

bool putCallback(char const *url, long data) {
	LOG_INFO_P("Main"); Serial.print("Put request to '"); Serial.print(url); Serial.printf("' arg: %ld\n", data);

	if (strcmp(url, "/led") == 0) {
		return true;
	} else if (strcmp(url, "/sound") == 0) {
		return true;
	} else if (strcmp(url, "/camera") == 0) {
		switch (data) {
			case 0:
				cameraModule.takePicture();
				return true;
			case 1:
				cameraModule.takePicture(~0u);
				return true;
			case 2:
				cameraModule.takeVideo();
				return true;
		}
	} else if (strcmp(url, "/time") == 0) {
		rtcModule.setTime(DateTime(static_cast<uint32_t>(data)));
		return true;
	} else if (strcmp(url, "/altitude") == 0) {
		bmpModule.setKnownAltitude(data / 1000.0f);
		return true;
	}

	return false;
}

void loop() {
	static unsigned long lastGpsRead = ~0u;

	psat::Data data = { 0 };

	data.millis = millis();

	gpsModule.writeData(data);

	if (data.gps.fix && data.millis - lastGpsRead > 10000) {
		LOG_INFO("Main", "Used Gps to set RTC");

		auto &gpsTime = data.gps.time;
		rtcModule.setTime(DateTime(gpsTime.y, gpsTime.mo, gpsTime.d, gpsTime.h, gpsTime.m, gpsTime.s));
		
		lastGpsRead = data.millis;
	}

	wiFiModule.writeData(data);
	rtcModule.writeData(data);
	imuModule.writeData(data);
	bmpModule.writeData(data);
	cameraModule.runCameraTasks();
	cameraModule.writeData(data);

	cardModule.writeToCard(data);
	wiFiModule.processClient(data, putCallback);
}
