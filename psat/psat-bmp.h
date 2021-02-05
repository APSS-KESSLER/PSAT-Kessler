#include <Adafruit_BMP280.h>

#ifndef PSAT_BMP_H
#define PSAT_BMP_H

namespace psat {

struct Data;

class BmpModule {
	Adafruit_BMP280 bmp;
	bool valid = false;
	float pressureAtSealevel = 1013.25f;

public:
	void setup();
	void setKnownAltitude(float altitude);
	void writeData(psat::Data &data);
};

}

#endif