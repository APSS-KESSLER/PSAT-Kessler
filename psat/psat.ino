#include "psat-wifi.h"
#include "psat-data.h"
#include "psat-log.h"
#include "psat-rtc.h"
#include "psat-sd.h"
#include "psat-imu.h"
#include "psat-gps.h"
#include "psat-bmp.h"
#include "psat-camera.h"
#include "psat-led.h"
#include "psat-buz.h"

psat::WiFiModule wiFiModule;
psat::RtcModule rtcModule;
psat::CardModule cardModule;
psat::ImuModule imuModule;
psat::GpsModule gpsModule;
psat::BmpModule bmpModule;
psat::CameraModule cameraModule;
psat::LedModule ledModule;
psat::BuzModule buzModule;

bool waitForGps = false;
unsigned long stopWaitingForGps;
const int led = 13;
bool armed = true;

int stage = 0;

void setup() {
	delay(3000);

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
	ledModule.setup();
	buzModule.setup();

	bmpModule.setKnownAltitude(0.0f);
}

bool putCallback(char const *url, long data) {
	LOG_INFO_P("Main"); Serial.print("Put request to '"); Serial.print(url); Serial.printf("' arg: %ld\n", data);

	if (strcmp(url, "/led") == 0) {
		ledModule.setLED(static_cast<int>(data));
		return true;
	} else if (strcmp(url, "/sound") == 0) {
		buzModule.selectSong(static_cast<int>(data));

		if (data == 2) {
			ledModule.setLED(3);
		}

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
		stage = 0;
		return true;
	}

	return false;
}

void doStageLogic(psat::Data &data) {
	float altitude = 0.0f;
	float acc = 0.0f;

	float rawAcc = data.acceleration.x * data.acceleration.x + data.acceleration.y * data.acceleration.y + data.acceleration.z * data.acceleration.z;
	altitude = altitude * 0.5f + data.pressureAltitude * 0.5f;
	acc = acc * 0.75f + rawAcc * 0.25f;

	LOG_INFO_F("Main", "Running altitude: %fm acceleration: %f(ms-2)^2", altitude, acc);
	LOG_INFO_F("Main", "Stage: %d", stage);

	data.stage = stage;

	switch (stage) {
	case 0:
		if (altitude > 100.0) {
			stage++;
		}

		break;
	case 1:
		if (acc < 400.0) {
			stage++;
			cameraModule.takePicture(100);
			cameraModule.takeVideo(30000);
		} else if (altitude < 50.0) {
			stage++;
		}

		break;
	case 2:
		if (altitude < 50.0) {
			stage++;
			cameraModule.takeVideo(30000);
		}

		break;
	}
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

	ledModule.showLEDs();
	wiFiModule.writeData(data);
	rtcModule.writeData(data);
	imuModule.writeData(data);
	bmpModule.writeData(data);

	if (armed) {
		doStageLogic(data);
	}

	cameraModule.runCameraTasks();
	cameraModule.writeData(data);
	buzModule.loop();

	cardModule.writeToCard(data);
	wiFiModule.processClient(data, putCallback);
}
