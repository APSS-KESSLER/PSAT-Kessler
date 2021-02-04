#ifndef PSAT_GPS_H
#define PSAT_GPS_H

#include <Adafruit_GPS.h>

namespace psat {

struct Data;

class GpsModule {
	Adafruit_GPS gps;
	bool valid = false;

public:
	GpsModule() : gps(&Serial1) {}

	void setup();
	void writeData(psat::Data &data);
};

}

#endif