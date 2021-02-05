#include "psat-bmp.h"
#include "psat-log.h"
#include "psat-data.h"

namespace psat {

void BmpModule::setup() {
	if (!bmp.begin()) {
		LOG_ERROR("BMP", "No atmospheric sensor found");
		return;
	}

	LOG_INFO("BMP", "Atmospheric sensor started");

	bmp.setSampling();
	valid = true;

	delay(1000);
}

void BmpModule::setKnownAltitude(float altitude) {
	if (!valid) {
		return;
	}

	pressureAtSealevel = bmp.seaLevelForAltitude(altitude, bmp.readPressure() / 100.0);
}

void BmpModule::writeData(psat::Data &data) {
	if (!valid) {
		return;
	}

	data.temperature = bmp.readTemperature();
	data.pressure = bmp.readPressure();
	data.pressureAltitude = bmp.readAltitude(pressureAtSealevel);
}

}