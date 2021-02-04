#include "psat-gps.h"
#include "psat-data.h"
#include "psat-log.h"

namespace psat {

void GpsModule::setup() {
	if (!gps.begin(9600)) {
		LOG_ERROR("GPS", "Failed to start GPS");
		return;
	}

	gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
	gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

	LOG_INFO("GPS", "Enabled GPS at 1HZ");

	valid = true;
}

void GpsModule::writeData(psat::Data &data) {
	if (!valid) {
		return;
	}

	while (!gps.newNMEAreceived() && gps.read() != 0);
	
	if (gps.newNMEAreceived()) {
		char *lastNmeaString = gps.lastNMEA();
		
		if (!gps.parse(lastNmeaString)) {
			LOG_ERROR_P("GPS"); Serial.printf("Unable to parse NMEA string '%s'", lastNmeaString);
			return;
		}
		
		LOG_INFO_F("GPS", "%s", lastNmeaString);
	}

	auto &o = data.gps;

	o.time.h = gps.hour;
	o.time.m = gps.minute;
	o.time.s = gps.seconds;
	o.time.ms = gps.milliseconds;
	o.fix = gps.fix;
	o.location.latitude = gps.latitudeDegrees;
	o.location.lat = gps.lat;
	o.location.longitude = gps.longitudeDegrees;
	o.location.lon = gps.lon;
	o.location.altitude = gps.altitude;
	o.speed = gps.speed;
	o.satellites = gps.satellites;
}

}