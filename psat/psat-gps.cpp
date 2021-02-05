#include "psat-gps.h"
#include "psat-data.h"
#include "psat-log.h"

namespace psat {

void GpsModule::setup() {
	if (!gps.begin(9600)) {
		LOG_ERROR("GPS", "Failed to start GPS");
		return;
	}

	*gps.lastNMEA() = '\0';
	gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

	LOG_INFO("GPS", "Enabled GPS at 1HZ");

	valid = true;
}

void GpsModule::writeData(psat::Data &data) {
	if (!valid) {
		return;
	}

	while (gps.read() != 0) {
		if (gps.newNMEAreceived()) {
			char *lastNmeaString = gps.lastNMEA();
		
			if (!gps.parse(lastNmeaString)) {
				LOG_ERROR("GPS", "Unable to parse NMEA string");
			}
		
			LOG_INFO_P("GPS"); Serial.print(lastNmeaString);
		}
	}

	auto &o = data.gps;

	o.time.y = gps.year;
	o.time.mo = gps.month;
	o.time.d = gps.day;
	o.time.h = gps.hour;
	o.time.m = gps.minute;
	o.time.s = gps.seconds;
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
