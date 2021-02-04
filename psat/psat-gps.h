#ifndef PSAT_GPS_H
#define PSAT_GPS_H

#include <Adafruit_GPS.h>
#include <Adafruit_PMTK.h>
#include <NMEA_data.h>

namespace psat {

struct Data;

class GpsModule {
	Adafruit_GPS gps;

public:
	GpsModule() : gps(&Serial1) {}

	void setup();
	void writeData(psat::Data &data);
};

}

#endif