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

bool waitForGps = false;
unsigned long stopWaitingForGps;
const int led = 13;
bool armed = false;

void setup() {
	pinMode(led, OUTPUT);
	digitalWrite(led, LOW);

	Serial.begin(115200);

	delay(10000);

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
	} else if (strcmp(url, "/gps") == 0) {
		waitForGps = true;
		stopWaitingForGps = millis() + static_cast<unsigned long>(data);
		wiFiModule.disconnect();
		return true;
	} else if (strcmp(url, "/arm") == 0) {
		armed = data == 0;
		return true;
	}

	return false;
}

void loop() {
	static unsigned long lastGpsRead = ~0u / 2;
	static unsigned long lastLEDToggle = ~0u / 2;
	static bool ledOn = false;

	psat::Data data = { 0 };

	data.millis = millis();

	gpsModule.writeData(data);

	if (armed && data.millis - lastLEDToggle > (waitForGps ? 1500 : 500)) {
		digitalWrite(led, ledOn ? LOW : HIGH);
		ledOn = !ledOn;
	}

	if (waitForGps) {
		if (data.gps.fix || stopWaitingForGps - data.millis > (~0u / 2)) {
			wiFiModule.setup();
			waitForGps = false;
		}

		return;
	}

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
